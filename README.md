# Rectangle Detection, Warping, and OCR

This C++ script detects rectangles in an image, warps the largest one (typically a document), and performs Optical Character Recognition (OCR) on the document.

## 1. Overview of the Code

The code performs the following tasks:

- Detects rectangles within an image.
- Identifies the largest rectangle, which is assumed to be the target document.
- Warps the perspective of the document to obtain a clear, flat view.
- Performs OCR on the warped document to extract text.

## 2. Prerequisites

To run the code, you will need:

- C++ installed on your system.
- OpenCV, Tesseract, and Leptonica libraries.

Here are guides to installing these prerequisites on different platforms:

- [Xcode (Mac)](https://www.geeksforgeeks.org/how-to-install-opencv-for-c-on-macos/)
- [Windows](https://www.tutorialspoint.com/how-to-install-opencv-for-cplusplus-in-windows)
- [Linux](https://www.geeksforgeeks.org/how-to-install-opencv-in-c-on-linux/)

## 3. Image Capture Guidelines

To ensure accurate detection and OCR, follow these guidelines when capturing the document image:

- Avoid placing the document on a surface with a similar color.
- Ensure all edges of the document are visible within the image.
- Capture the image with sufficient quality and lighting.

## 4. Output

The detected text is stored in the `outText` variable. The script currently outputs the text using `std::cout`, but you can modify the code to save the text in a file or perform other operations as needed.
