
# 🧭 Maze Game - WebAssembly Edition

## 🚀 Introduction
**Maze Game** is a 3D first-person maze navigation game built in **C++** and compiled to **WebAssembly** using **Emscripten**. Play directly in your browser with smooth and immersive controls!

## 🎮 Play Online
Try the live version now:  
🔗 [zreechxnn.github.io/maze-game](https://zreechxnn.github.io/maze-game/)

## ✨ Features  
- ⚡ **Powered by WebAssembly** for high performance  
- 🧠 **Challenging Maze Levels**  
- ⏱️ **Timer System** to track progress  
- 🏆 **Score System** based on performance  
- 🖥️ **Fullscreen Mode** for better immersion  

## 📺 Demo Video
[![Maze Game Demo](https://img.youtube.com/vi/GKmmAStua-A/maxresdefault.jpg)](https://youtu.be/GKmmAStua-A?si=9-S11YBn6k-NVCba)  
➡️ *Click the image to watch the gameplay demo on YouTube!*

## 🛠️ Installation & Local Setup

### 🔧 Quick Start (recommended):
```bash
git clone https://github.com/Zreechxnn/maze-game.git
cd maze-game
./run.sh
````

### 🛠 Manual Setup:

```bash
python3 -m http.server
# Then open in your browser: http://localhost:8000
```

> Ensure you have Python 3 installed.

**Goal:** Reach the maze exit as fast as possible — avoid dead ends and beat your best time!

## 🧩 Game Mechanics

* 🕒 **Time Tracking** to improve speedruns
* 📈 **Level Difficulty** increases with progress
* 🧮 **Score System** rewards efficient navigation

## 🤝 Contributing

We welcome contributions from the community:

1. Fork the repository
2. Create a new branch:
   `git checkout -b feature/your-feature`
3. Commit your changes:
   `git commit -am 'Add some feature'`
4. Push your branch:
   `git push origin feature/your-feature`
5. Create a Pull Request (PR)

## 🔗 Related Projects

* [3D WebGL Maze Generator](https://github.com/example/maze-generator)
* [WebAssembly Game Framework](https://github.com/example/wasm-game-framework)
* [First-Person WebGL Engine](https://github.com/example/webgl-engine)

## 📜 License

This project is licensed under the **MIT License**. See the [LICENSE](LICENSE) file for more info.

---

Thanks for checking out **Maze Game**! If you enjoy it, don't forget to ⭐ the repo!

---

## 🖥 `run.sh` Script Example

```bash
#!/bin/bash
# Start a local server and open the game in browser

python3 -m http.server &

sleep 2

# For macOS
open http://localhost:8000

# Uncomment this line for Linux:
# xdg-open http://localhost:8000
```

> Don’t forget to make it executable:

```bash
chmod +x run.sh
```
