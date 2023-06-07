// keeps two handles
class SimplePipe {
public:
    SimplePipe() noexcept {
        sa_.lpSecurityDescriptor = &sd_;
        sa_.nLength = sizeof(SECURITY_ATTRIBUTES);
        sa_.bInheritHandle = TRUE;  // allow handle inherit for child process
    }

    SimplePipe(const SimplePipe &) = delete;
    SimplePipe &operator=(const SimplePipe &) = delete;
    SimplePipe(SimplePipe &&rhs) = delete;
    SimplePipe &operator=(SimplePipe &&rhs) = delete;

    ~SimplePipe() { shutdown(); }

    bool create() {
        // protected by lock
        std::lock_guard lk(lock_);
        if (read_ != nullptr || write_ != nullptr) {
            return true;
        }

        if (!sa_initialized_ && !initDescriptorsWithFullAccess()) {
            return false;  // really, something weird
        }

        if (::CreatePipe(&read_, &write_, &sa_, 0) == 0) {
            read_ = nullptr;
            write_ = nullptr;
            xlog::l("Failed to create pipe, %d", GetLastError()).print();
            return false;
        }

        // disable inheriting from the child
        if (SetHandleInformation(read_, HANDLE_FLAG_INHERIT, 0) == 0) {
            xlog::l("Failed to change handle information, %d", GetLastError())
                .print();
            ::CloseHandle(read_);
            ::CloseHandle(write_);
            read_ = nullptr;
            write_ = nullptr;
            return false;
        }
        xlog::v("Allocated  2 handle %p %p", read_, write_);
        return true;
    }

    void shutdown() noexcept {
        std::lock_guard lk(lock_);
        if (read_ != nullptr) {
            ::CloseHandle(read_);
            read_ = nullptr;
        }
        if (write_ != nullptr) {
            ::CloseHandle(write_);
            write_ = nullptr;
        }
    }

    HANDLE getRead() const noexcept {
        std::lock_guard lk(lock_);
        return read_;
    }
    HANDLE getWrite() const noexcept {
        std::lock_guard lk(lock_);
        return write_;
    }

    HANDLE moveWrite() noexcept {
        std::lock_guard lk(lock_);
        auto *write = write_;
        write_ = nullptr;
        return write;
    }

private:
    bool initDescriptorsWithFullAccess() {
        auto ret =
            ::InitializeSecurityDescriptor(&sd_, SECURITY_DESCRIPTOR_REVISION);
        if (ret == 0) {
            xlog::l(XLOG_FLINE + "Stupid fail").print();
            return false;
        }

        // *******************************************************
        // #TODO change access right to the owner of the process
        // below we have code from the winagent, which grants any access to
        // the object this is quite dangerous
        // NOW THIS IS BY DESIGN of Check MK
        // https://docs.microsoft.com/de-at/windows/desktop/SecAuthZ/creating-a-security-descriptor-for-a-new-object-in-c--
        // ******************************************************
        ret = ::SetSecurityDescriptorDacl(&sd_, 1, nullptr, 0);  // NOLINT
        if (ret == 0) {
            xlog::l(XLOG_FLINE + "Not so stupid fail %d", GetLastError())
                .print();
            return false;
        }
        sa_initialized_ = true;
        return true;
    }
    mutable std::mutex lock_;
    HANDLE read_{nullptr};
    HANDLE write_{nullptr};
    bool sa_initialized_{false};
    SECURITY_DESCRIPTOR sd_ = {};
    SECURITY_ATTRIBUTES sa_ = {};
};




TEST(PlayerTest, Pipe) {
    const auto p = std::make_unique<SimplePipe>();
    EXPECT_EQ(p->getRead(), nullptr);
    EXPECT_EQ(p->getWrite(), nullptr);
    p->create();
    EXPECT_NE(p->getRead(), nullptr);
    EXPECT_NE(p->getWrite(), nullptr);
}


TEST(Wtools, SimplePipeBase) {
    SimplePipe pipe;
    EXPECT_EQ(pipe.getRead(), nullptr);
    EXPECT_EQ(pipe.getWrite(), nullptr);

    pipe.create();
    EXPECT_NE(pipe.getRead(), nullptr);
    EXPECT_NE(pipe.getWrite(), nullptr);

    const auto write_handle = pipe.getWrite();
    const auto handle = pipe.moveWrite();
    EXPECT_EQ(pipe.getWrite(), nullptr);
    EXPECT_EQ(handle, write_handle);

    pipe.shutdown();
    EXPECT_EQ(pipe.getRead(), nullptr);
    EXPECT_EQ(pipe.getWrite(), nullptr);
}

