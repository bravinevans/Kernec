# KERNEC-0005 — Coding Standard

**Document ID:** KERNEC-0005
**Revision:** 1.0
**Title:** Kernec Operating System — Coding Standard
**Status:** Approved
**Date:** 2026-07-01
**Author:** Lead Implementation Engineer

---

# 1. Purpose

This specification defines the mandatory coding conventions used throughout the Kernec project.

Consistency SHALL take precedence over personal preference.

---

# 2. General Principles

Code SHALL be:

- readable
- maintainable
- deterministic
- documented
- testable

Every change SHOULD leave the codebase in a better state than it was found.

---

# 3. Languages

Primary languages:

- C23
- C++23
- Rust (selected components)
- POSIX Shell
- Python 3

---

# 4. Formatting

- UTF-8 encoding
- Unix line endings (LF)
- Four spaces for indentation
- No tab characters
- Maximum line length: 100 characters

Formatting SHALL be automated.

---

# 5. Naming

Variables:

snake_case

Functions:

snake_case

Macros:

UPPER_CASE

Constants:

UPPER_CASE

Structures:

PascalCase

Enums:

PascalCase

Files:

lowercase-with-hyphens

---

# 6. Comments

Comments SHALL explain:

- why

Comments SHOULD NOT explain:

- obvious code behavior

---

# 7. Error Handling

Every recoverable error SHALL return meaningful status information.

Fatal errors SHALL terminate cleanly with diagnostic output.

---

# 8. Logging

Logging levels:

- TRACE
- DEBUG
- INFO
- WARN
- ERROR
- FATAL

---

# 9. Testing

Every major component SHALL include unit tests where practical.

Integration tests SHALL reside under:

tests/

---

# 10. Documentation

Every public API SHALL be documented.

Every subsystem SHALL include a README describing its purpose.

---

# 11. Static Analysis

Official builds SHALL pass:

- clang-format
- clang-tidy

No warnings SHALL exist in Release builds.

---

# 12. Code Review

Every pull request SHALL:

- compile successfully
- follow formatting rules
- pass automated tests
- receive review before merging

---

End of KERNEC-0005 Revision 1.0
