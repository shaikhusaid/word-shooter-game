# word-shooter-game
A fast-paced 3D word shooter game built in C++ using GLEW and GLUT.
# Word Shooter Game 🎯

**A fast-paced 2D word puzzle shooter game built in C++ with OpenGL, GLEW, and GLUT.**  
This project combines **graphics programming**, **file handling**, **algorithmic thinking**, and **real-time interaction** to deliver an engaging educational arcade experience.

---

## 🎮 Overview
Word Shooter challenges players to test their vocabulary under time pressure.  
Shoot letters into the grid, form valid words from a massive **370,099-word dictionary**, and score as high as possible before the timer runs out.  

The game blends:
- Real-time graphics rendering with OpenGL
- Complex word matching algorithms
- File handling for loading a large dataset efficiently
- Interactive controls using the mouse

---

## ✨ Features
- **3,70,099+ Words Dictionary** (loaded via file handling)
- **OpenGL Graphics Rendering** (GLEW + GLUT)
- Smooth animations & dynamic board updates
- Mouse-based targeting and shooting
- Word detection in **horizontal, vertical, and diagonal** directions
- Burst animations for valid words
- Score tracking and time-limited gameplay
- Background music integration

---

## 🛠️ Technologies Used
- **Language:** C++
- **Graphics:** OpenGL, GLEW, GLUT
- **Development Tools:** g++, Makefile
- **Data:** Text file dictionary with over 3 lakh words
- **OS:** Linux (Ubuntu recommended)

---

## 🚀 Installation & Run
```bash
# Install dependencies (Ubuntu)
sudo apt-get install libglu1-mesa-dev freeglut3-dev libglew-dev

# Clone the repository
git clone https://github.com/shaikhusaid/word-shooter-game.git
cd word-shooter-game

# Build the game
make

# Run the game
./word-shooter
