# Contributing to AlderKernel

Thank you for your interest in contributing to AlderKernel!

AlderKernel is a hobby monolithic kernel created for learning and experimentation.

## How to contribute

There are several ways you can help:

- Report bugs
- Suggest new features
- Improve documentation
- Fix code issues / Optimize code
- Add drivers or kernel features
- Improve existing systems

## Reporting bugs

Before reporting a bug:

- Make sure you are using the latest version.
- Check if the issue has already been reported.
- Provide as much information as possible.
- Provide a way how someone can reproduce the bug.

A good bug report should include:

- What happened
- What you expected to happen
- Steps to reproduce the problem
- Build environment
- Error messages or screenshots

## Suggesting features

Feature suggestions are welcome.

Please explain:

- What the feature does
- Why it would be useful
- How it could be implemented (if you have ideas)

## Code contributions

Before submitting code:

- Keep the code readable and avoid inappropriate comments or messages.
- Follow the existing coding style.
- Add comments where needed.
- Test your changes before submitting.
- Make sure your contribution follows the project's license.

For kernel-related changes, please consider:

- Avoid breaking existing functionality.
- Keep architecture-specific code separated when possible.
- Make changes focused and easy to review.

## Kernel development guidelines

When working on kernel code:

- Avoid unnecessary dependencies.
- Keep hardware-specific code isolated.
- Consider memory safety and performance.
- Document important design decisions.

## Building the project

Requirements:

- NASM
- GCC
- GNU Make
- GNU ld
- QEMU

Build:

```bash
make
```

Run:

```bash
make run
```

## Pull requests

Before submitting:

1. Fork the repository.
2. Create a branch for your changes.
3. Test your changes.
4. Submit a pull request.

When creating a pull request:

- Explain what was changed.
- Explain why the change was needed.
- Mention any known limitations.
- Keep commits clear and descriptive.

## Code of conduct

- Please be respectful to other contributors.
- Discussions, questions, and different ideas are welcome as long as they remain constructive.

Thank you for helping improve AlderKernel!