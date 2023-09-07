# ** WARNING ** THIS DIRECTORY IS DEPRECATED AND WILL BE REMOVED END OF 2023
# ** WARNING ** USE tests/saithriftv2 and sonic-net/DASH repo for bvm2/p4 model

# SAI BEHAVIORAL MODEL
This repository clearly defines widely accepted network switch behaviour.
The switch behavioral model provides a commom framework for all network vendors and users.
It meant to describe the standard ethernet switch features and flows.
The model enables for both users and vendors to experiance the SAI switch model using SAI API over P4 soft switch.

## Repository Structure
The repository consist of:

### [P4 soft-switch](p4-sai/)
P4_14 source files to be complied by the p4c complier 
Can be independently configured via the CLI interface.
### [SAI adapter](sai_adapter/)
C source files implementation for the SAI API build for P4 soft switch model.
SAI lib
Tests frameworks: PTF, unittests.
