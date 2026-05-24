# 🎭 LSB Image Steganography

A C-based **LSB (Least Significant Bit) Image Steganography** project developed to securely hide and retrieve secret text data inside BMP images using binary file handling and bit manipulation techniques.

---

# 📖 Project Overview

Steganography is an ancient technique used to secretly hide information inside another medium so that the existence of the message itself remains unnoticed. Earlier, hidden messages were concealed using invisible ink, paintings, and handwritten documents. With the advancement of digital technology, this concept evolved into Digital Steganography, where secret data can be hidden inside images, audio, or video files.

This project implements **Image Steganography using the LSB technique**, where secret text data is embedded into the least significant bits of pixel values in a BMP image.

The application supports:

* Encoding secret text into BMP images
* Decoding hidden text from encoded images
* Command-line based execution
* Binary-safe image processing
* Preservation of image appearance after encoding

---

# 🎯 Problem Statement

Sharing confidential information openly can lead to privacy and security issues.

Traditional encryption protects data but may still attract attention. Steganography helps overcome this by hiding the existence of the data itself.

Challenges addressed by this project:

-> Securely hiding secret information inside images
-> Preventing visible distortion of image quality
-> Efficient binary-level image manipulation
-> Safe encoding and decoding of hidden data
-> Lightweight command-line utility for steganography

This project solves these problems by providing:

-> Efficient LSB-based data hiding
-> Secure secret message retrieval
-> Binary-safe BMP file operations
-> Simple command-line interaction
-> Minimal visual changes to images

---

# 🚀 Objectives of the Project

-> To understand image steganography concepts
-> To implement binary file handling in C
-> To practice bitwise operations and data manipulation
-> To understand BMP image structure
-> To develop modular programming skills
-> To improve debugging and problem-solving abilities
-> To implement real-world data hiding techniques

---

# ✨ Features

## 🔐 Encode Secret Data

* Hide secret text inside BMP images
* Supports `.txt` secret files
* Preserves image quality after encoding

## 🔓 Decode Hidden Data

* Extract hidden text from encoded BMP images
* Restores the original secret message accurately

## 🖥️ Command-Line Based Execution

* Simple and lightweight terminal interface
* Easy encoding and decoding operations

## 💾 Binary File Processing

* Safe BMP image handling
* Byte-level data manipulation
* LSB bit replacement technique

---

# 🛠️ Tools & Technologies Used

| Tool / Technology    | Purpose                      |
| -------------------- | ---------------------------- |
| C Programming        | Core application development |
| GCC Compiler         | Compiling source code        |
| Linux / WSL          | Development environment      |
| Binary File Handling | BMP image processing         |
| Bitwise Operations   | LSB encoding & decoding      |
| BMP File Format      | Image data manipulation      |

---

# 🧠 Concepts Used

-> File Handling
-> Binary File Processing
-> Bitwise Operations
-> Structures
-> Functions
-> Pointers
-> Command Line Argument Parsing
-> Modular Programming
-> Image Steganography
-> LSB Encoding Technique

---

# 📂 Project Structure

```text id="2mbn18"
LSB_Image_Stegnography/
│
├── main.c
├── encode.c
├── decode.c
├── encode.h
├── decode.h
├── common.h
├── types.h
│
├── beautiful.bmp
├── sec.txt
├── stego.bmp
├── dec_data.txt
│
└── README.md
```

---

# ⚙️ How to Compile and Run

## 🔹 Compile

```bash id="pbyv0f"
gcc *.c
```

---

# ▶️ Encoding

Hide secret data inside a BMP image:

```bash id="bdg2vi"
./a.out -e beautiful.bmp sec.txt stego.bmp
```

### Parameters

* `beautiful.bmp` → Original image
* `sec.txt` → Secret text file
* `stego.bmp` → Encoded output image

---

# ▶️ Decoding

Extract hidden data from encoded image:

```bash id="p6l5b5"
./a.out -d stego.bmp
```

### Output

Decoded secret message is stored in:

```text id="4ov6zq"
dec_data.txt
```

---

# 💾 File Handling

The project uses binary file handling to:

* Read BMP image headers
* Access pixel data safely
* Modify least significant bits
* Embed secret text into image bytes
* Extract hidden data accurately
* Preserve image structure after encoding

---

# 📸 Sample Execution

## Encoding

```bash id="9d4p9n"
./a.out -e beautiful.bmp sec.txt stego.bmp
```

## Decoding

```bash id="3i0gz2"
./a.out -d stego.bmp
```

## Decoded Output

```text id="wqf1o3"
Coordinates confirmed. Proceed silently.
```

---

# 🎓 Learning Outcomes

Through this project, the following skills were improved:

-> Image steganography concepts
-> Binary file handling in C
-> BMP image structure understanding
-> Bitwise data manipulation
-> Command-line application development
-> Modular programming techniques
-> Debugging and testing skills
-> Secure data hiding concepts

---

# 🔮 Future Enhancements

-> GUI-based steganography application
-> Support for PNG and JPEG formats
-> Password-protected encoding
-> Audio and video steganography support
-> Encryption before encoding
-> Multiple file hiding support

---

# 📝 Note

This project currently supports only **BMP image format** because BMP files allow direct pixel-level manipulation without compression.

Compressed image formats may require different encoding techniques.

---

# 👨‍💻 Author

Mohammed Maaz

LinkedIn:
[www.linkedin.com/in/mdm-mohammedmaaz](https://www.linkedin.com/in/mdm-mohammedmaaz)

---

# 📜 License

This project is developed for educational and learning purposes.
