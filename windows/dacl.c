// limit access to the folder

void test_ref() {
    DWORD dwRes;
    PSID pEveryoneSID = NULL, pAdminSID = NULL;
    PACL pACL = NULL;
    PSECURITY_DESCRIPTOR pSD = NULL;
    EXPLICIT_ACCESS ea[2];
    SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;
    SID_IDENTIFIER_AUTHORITY SIDAuthNT = SECURITY_NT_AUTHORITY;
    SECURITY_ATTRIBUTES sa;

    //    HKEY hkSub = NULL;

    // Create a well-known SID for the Everyone group.
    if (!AllocateAndInitializeSid(&SIDAuthWorld, 1, SECURITY_WORLD_RID, 0, 0, 0,
                                  0, 0, 0, 0, &pEveryoneSID)) {
        //(("AllocateAndInitializeSid Error %u\n"), GetLastError());
        goto Cleanup;
    }

    // Initialize an EXPLICIT_ACCESS structure for an ACE.
    // The ACE will allow Everyone read access to the key.
    ZeroMemory(&ea, 2 * sizeof(EXPLICIT_ACCESS));
    ea[0].grfAccessPermissions = GENERIC_READ;
    ea[0].grfAccessMode = SET_ACCESS;
    ea[0].grfInheritance = NO_INHERITANCE;
    ea[0].Trustee.TrusteeForm = TRUSTEE_IS_SID;
    ea[0].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
    ea[0].Trustee.ptstrName = (LPTSTR)pEveryoneSID;

    // Create a SID for the BUILTIN\Administrators group.
    if (!AllocateAndInitializeSid(&SIDAuthNT, 2, SECURITY_BUILTIN_DOMAIN_RID,
                                  DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0,
                                  &pAdminSID)) {
        //(("AllocateAndInitializeSid Error %u\n"), GetLastError());
        goto Cleanup;
    }

    // Initialize an EXPLICIT_ACCESS structure for an ACE.
    // The ACE will allow the Administrators group full access to
    // the key.
    ea[1].grfAccessPermissions = GENERIC_ALL;
    ea[1].grfAccessMode = SET_ACCESS;
    ea[1].grfInheritance = NO_INHERITANCE;
    ea[1].Trustee.TrusteeForm = TRUSTEE_IS_SID;
    ea[1].Trustee.TrusteeType = TRUSTEE_IS_GROUP;
    ea[1].Trustee.ptstrName = (LPTSTR)pAdminSID;

    // Create a new ACL that contains the new ACEs.
    dwRes = SetEntriesInAcl(2, ea, NULL, &pACL);
    if (ERROR_SUCCESS != dwRes) {
        //(("SetEntriesInAcl Error %u\n"), GetLastError());
        goto Cleanup;
    }

    // Initialize a security descriptor.
    pSD =
        (PSECURITY_DESCRIPTOR)LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
    if (NULL == pSD) {
        //(("LocalAlloc Error %u\n"), GetLastError());
        goto Cleanup;
    }

    if (!InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION)) {
        //(("InitializeSecurityDescriptor Error %u\n"), GetLastError());
        goto Cleanup;
    }

    // Add the ACL to the security descriptor.
    if (!SetSecurityDescriptorDacl(pSD,
                                   TRUE,  // bDaclPresent flag
                                   pACL,
                                   FALSE))  // not a default DACL
    {
        //(("SetSecurityDescriptorDacl Error %u\n"), GetLastError());
        goto Cleanup;
    }

    // Initialize a security attributes structure.
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = pSD;
    sa.bInheritHandle = FALSE;

    // Use the security attributes to set the security descriptor
    // when you create a key.
    //    RegCreateKeyExW(HKEY_CURRENT_USER, L"mykey", 0, L"", 0,
    //                    KEY_READ | KEY_WRITE, &sa, &hkSub, &dwDisposition);
    CreateDirectoryA("c:\\temp\\example.exe.ref", &sa);

    //    CreateFileA("c:\\temp\\example.exe", GENERIC_EXECUTE, 0, &sa,
    //    CREATE_NEW,
    //                FILE_ATTRIBUTE_NORMAL, 0);
    //(("RegCreateKeyEx result %u\n"), lRes);

Cleanup:

    if (pEveryoneSID) FreeSid(pEveryoneSID);
    if (pAdminSID) FreeSid(pAdminSID);
    if (pACL) LocalFree(pACL);
    if (pSD) LocalFree(pSD);

    return;
}
