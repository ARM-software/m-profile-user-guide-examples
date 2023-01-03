<!--
SPDX-FileCopyrightText: Copyright 2022 Arm Limited <open-source-office@arm.com>
SPDX-License-Identifier: MIT
-->

# Armv8-M Architecture - User Guide Examples

## Introduction

The Armv8-M architecture defines many aspects of a Cortex-M processor’s behavior. Some major architecture extension categories include:

- Programmers’ model
- Exception model
- Memory model
- Security extension
- Debug components

For more details on Armv8-M architecture rules and its pseudocode, please refer to [Armv8-M Architecture Reference Manual](https://developer.arm.com/documentation/ddi0553/latest) 

To assist users in understanding the Armv8-M architecture features, a set of user guides are developed to describe the architecture extension categories shown above. These user guides are written in simple language such that it is easy to learn and help users understand the architectural features along with example project source code (wherever applicable). The user guides also provide details on: 

- Linkage between an architecture feature and its actual usage in the software 
- Guidelines/recommendations for Software developers (Do's and Don'ts)

User guides for Armv8-M architecture are available at [Learn The Architecture - M-Profile](https://www.arm.com/architecture/learn-the-architecture/m-profile). Example projects are developed and linked with these user guides to give a practical explanation of architecture feature usage. This repository contains example projects that link with the user guide. Each example project given here should be referred along with the notes/explanations given in their corresponding user guide.

## Project Structure

The example software projects are organised as per their architecture extension category.

| Architecture category | User Guide | Chapter | Folder name |
| --------------------- | -----------| --------| ------------| 
| Memory Model  | [Armv8-M Memory model and MPU user guide](https://developer.arm.com/documentation/107565/) | [Use case examples](https://developer.arm.com/documentation/107565/0100/Use-case-examples) | [Memory_model/](https://github.com/ARM-software/m-profile-user-guide-examples/tree/main/Memory_model)

To understand the basics and general information about the example projects, refer to [Generic Information](https://developer.arm.com/documentation/107565/0100/Use-case-examples/Generic-Information). For details on each example project, refer to the Readme available within it.

Note: 
Example software projects for other architecture extension categories will be added in future releases.

## Additional Notes

- The example projects are developed to demonstrate the architectural features. These example projects should not be used as a test to validate all aspects of a feature.
- The example projects are expected to work with the set of tool versions. The toolchain version details are captured in each example project's Readme.md file. Though the example may work using other versions of a toolchain, it is not verified.

## License
Example projects are licensed under the MIT2.0 license. Please see the [LICENSE](LICENSE)

## Feedback and Support

- For feedback and support on these example projects, use the GitHub [Issue Tracker](https://github.com/ARM-software/m-profile-user-guide-examples/issues) that is associated with this repository.
- For support on Armv8-M architecture features, refer [developer](https://developer.arm.com/support).
- Arm licensees may contact Arm directly through their partner managers.


----

*Copyright 2022, Arm Limited*

