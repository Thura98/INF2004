# INF2004

# Project's Title
ByteBack : SD Card Recovery Tool

# Project Description
The project is a forensic recovery tool that utilizes Raspberry Pi Pico W and C language titled ByteBack. The tool operates at a low-level, beyond the filesystem and analyzes the raw data within the SD Card. There are two phases to the tool: Data Carving (Recovery) and Data Storage.

It performs a recovery operation on every single byte of data on the SD Card which is useful for recovering corrupted data. The recovered data is then transferred via the UART communication link from one Pico device to a second Pico device, which handles the Data Storage in a similar fashion to the Data Recovery where it reads the recovered data byte by byte and stored on the second Pico device. Another unique feature for ByteBack is that the device for Data Recovery has a Write Blocker that prevents accidental alteration of data being recovered in the process to ensure integrity of the data.

# How to Install and Run the Project
Hardware Required:
- Laptop/PC
- x2 Raspberry Pico W
- x2 Maker Pi Pico
- Male Jumper Cables
- 2x 32GB SD Card

# How to Use the Project
Provide instructions and examples so users/contributors can use the project.
Can include visual aids - eg: screenshot of running project, structure and design principle used

# Credits
Team Members:
- Thura: 
- Michelle: 
- Carment: https://github.com/Carment14tham
- Ming Yi: 
- Ivan: 
