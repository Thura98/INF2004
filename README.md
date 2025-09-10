# INF2004

# Project's Title
ByteBack : SD Card Recovery Tool

https://youtu.be/WRObqC7rqi8

# Project Description
The project is a forensic recovery tool that utilizes Raspberry Pi Pico W and C language titled ByteBack. The tool operates at a low-level, beyond the filesystem and analyzes the raw data within the SD Card. There are two phases to the tool: Data Carving (Recovery) and Data Storage.

It performs a recovery operation on every single byte of data on the SD Card which is useful for recovering corrupted data. The recovered data is then transferred via the UART communication link from one Pico device to a second Pico device, which handles the Data Storage in a similar fashion to the Data Recovery where it reads the recovered data byte by byte and stored on the second Pico device. Another unique feature for ByteBack is that the device for Data Recovery has a Write Blocker that prevents accidental alteration of data being recovered in the process to ensure integrity of the data.

# How to Install and Run the Project
Hardware Required:
- Laptop/PC with windows 10 and above
- x2 Raspberry Pico W
- x2 Maker Pi Pico
- x1 Male Jumper Cables
- 2x 32GB SD Card (Preferably SANDISK brand SD cards)

Software required:
- Development environment for the pico in C using the Pico SDK for windows 10 and above
**(Download and set up your environment using this link: https://github.com/raspberrypi/pico-setup-windows/releases/latest/download/pico-setup-windows-x64-standalone.exe)**

# How to Use the Project
Provide instructions and examples so users/contributors can use the project.
Can include visual aids - eg: screenshot of running project, structure and design principle used

1. Download and unzip the folder

![alt text](https://github.com/Carment14tham/INF2004/blob/main/img/Capture2.PNG)

2. Go to your pico examples folder(PATHTO\Documents\Pico-v1.5.1\pico-examples) and add the INF2004_GRP26_PROJECT folder

![alt text](https://github.com/Carment14tham/INF2004/blob/main/img/Capture.PNG)

3. Go to the overall CMakeLists.txt file and write "add_subdirectory(INF2004_GRP26_PROJECT)"  at the end of the 

![alt text](https://github.com/Carment14tham/INF2004/blob/main/img/Capture3.jpg)

4. To allow file carving to be performed on your SD card using disk read operations, build the **SD_FileCarve_Init** and run it on your raspberry pi pico w (Ensure your SD Card is a new one, the **data** on your **SD Card** will be **lost**!)

![alt text](https://github.com/Carment14tham/INF2004/blob/main/img/Capture4.PNG)
 
# Credits
Team Members:
- Thura(https://github.com/Thura98): Data Recovery and Identification
    - Function: Develop code to perform recovery of deleted files from SD card.
    - Coding Requirements: Utilize Pico to perform file carving to recover deleted files from the SD card and identify the type of files recovered.
- Michelle(https://github.com/soobdoobers42): Write Blocker
    - Function: Preventing the data from being tempered after File Carving is performed on the SD Card.
    - Coding Requirements: Create functions to block writing from flash memory.
- Carment(https://github.com/Carment14tham): Read/Write Operations
    - Function: Develop code for accessing, retrieving and updating the data in the SD card.
    - Coding Requirements: Ensuring the data can be accessed, retrieved and updated. Create functions for users to be able to perform read and write after the Write Blocker is implemented.
- Ming Yi(https://github.com/2201656): SD Card Access using SPI
    - Function: Develop code to read data from target SPI flash memory chips.
    - Coding Requirements: Implement SPI communication protocols to interact with the SPI flash memory. Create functions for reading, writing, and deleting data from the flash memory.
- Ivan: Data Transfer
    - Function: Coordinate the integration of coding modules developed by other team members.
    - Coding Requirements: Ensure that the different components of the tool, including data acquisition, storage, and GUI, work seamlessly together. Debug and resolve any compatibility issues.
 
# Challenges
## Limitations:
1. There is limited memory space in the Raspberry Pico to store multiple data files of various filesystem.
2. UART Connection has a high chance of data loss during data transfer.
3. Limited to omly image file size.
4. Multiple Library dependencies

# Deliverables
- A practical Embedded Forensic Tool built on the Raspberry Pi Pico.
- Well-documented source code.
- A detailed project report outlining the design, development process, technical challenges, and lessons learned.
- A presentation showcasing the tool's capabilities, its role in digital forensics, and potential applications in security research and investigations.

