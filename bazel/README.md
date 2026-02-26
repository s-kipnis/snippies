Here is the content as a proper **Markdown (.md) file**:

---

```markdown
# 🚀 Bazel 101 – Command Line Guide

A compact reference for typical daily Bazel commands (CLI-focused, mono-repo friendly).

---

## 0️⃣ Target Patterns (Most Important Concept)

```

//...                 → everything in repo
//pkg/...             → everything in pkg/
//pkg:target          → specific target
:target               → target in current directory

````

---

## 1️⃣ Build

Build everything:

```bash
bazel build //...
````

Build specific package:

```bash
bazel build //agents/wnx/...
```

Build single target:

```bash
bazel build //agents/wnx:my_binary
```

Verbose debugging:

```bash
bazel build //... --verbose_failures
```

---

## 2️⃣ Run

Run executable target:

```bash
bazel run //agents/wnx:my_binary
```

Pass arguments to the program:

```bash
bazel run //agents/wnx:my_binary -- --arg1 value
```

Everything after `--` is passed to the binary.

---

## 3️⃣ Test

Run all tests:

```bash
bazel test //...
```

Run tests in specific package:

```bash
bazel test //agents/wnx/...
```

Show full test output:

```bash
bazel test //... --test_output=all
```

Run a single test:

```bash
bazel test //agents/wnx:test_python_module
```

---

## 4️⃣ Type Checking (mypy via Build Config)

If mypy is integrated via aspect:

```bash
bazel build --config=mypy //...
```

Type-check specific module:

```bash
bazel build --config=mypy //agents/wnx/...
```

Verbose output:

```bash
bazel build --config=mypy //... --verbose_failures
```

---

## 5️⃣ Lint / Format (Depends on Repo Setup)

Common patterns:

```bash
bazel run //:format
bazel run //:lint
```

Or:

```bash
bazel test //:lint
```

Find lint/format targets:

```bash
bazel query //... | grep lint
```

---

## 6️⃣ Query (Very Powerful)

List all targets:

```bash
bazel query //...
```

List all tests:

```bash
bazel query 'kind("test", //...)'
```

Find specific rule type:

```bash
bazel query 'kind("py_library", //...)'
```

Show dependencies:

```bash
bazel query 'deps(//agents/wnx:my_binary)'
```

Reverse dependencies:

```bash
bazel query 'rdeps(//..., //agents/wnx:my_binary)'
```

---

## 7️⃣ Clean

Soft clean:

```bash
bazel clean
```

Full reset:

```bash
bazel clean --expunge
```

---

## 8️⃣ Useful Debug Flags

```
--verbose_failures
--sandbox_debug
--test_output=all
--jobs=1
```

Example:

```bash
bazel test //... --verbose_failures --test_output=all
```

---

## 9️⃣ Typical Python Mono-Repo Workflow

```bash
bazel build //...
bazel test //...
bazel build --config=mypy //...
bazel run //:format
```

---

## 🔟 Inspect Configurations

See available build options:

```bash
bazel help build
```

Inspect `.bazelrc` to understand what `--config=mypy` expands to.

---

## 🧠 Mental Model

Bazel always:

1. Loads `BUILD` files
2. Creates a dependency graph
3. Executes actions in sandbox
4. Caches everything

If something is slow, broken, or weird → think **graph + cache**.

