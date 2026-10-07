# 🚀 Galaxy Defender

A 2D space survival game developed in **C++ using OpenGL/GLUT**.

The player controls a spaceship at the bottom of the screen and must destroy incoming enemies while surviving for as long as possible. The game includes shooting, enemy movement, collision detection, lives, score, increasing difficulty, pause/resume, restart, and file handling for storing previous player records.

---

## 🎮 Game Objective

The objective of **Galaxy Defender** is to survive for as long as possible while destroying incoming enemies.

The player must:

- 🚀 Control the spaceship
- 🔫 Shoot incoming enemies
- 💥 Destroy enemies to increase the score
- ❤️ Maintain the available lives
- 🏆 Try to achieve the highest possible score

The game is survival-based and continues until the player's lives reach zero.

---

## 🎮 Controls

| Key | Action |
|---|---|
| `←` | Move spaceship left |
| `→` | Move spaceship right |
| `SPACE` | Shoot bullet |
| `P / p` | Pause game |
| `R / r` | Resume game |
| `A / a` | Restart game |
| `B / b` | View previous records |
| `ENTER` | Start game |
| `ESC` | Exit game |

The spaceship is restricted to the game boundaries and can move horizontally across the playable area.

---

## 🛸 Player

The player controls a spaceship located near the bottom of the screen.

### Player features

- Starts with **3 lives**
- Moves horizontally
- Cannot move outside the boundaries
- Can fire bullets using the `SPACE` key
- Loses a life when an enemy reaches the bottom or collides with the player

---

## 🔫 Bullet System

The game uses an array to store bullets.

Bullets:

- Move upward
- Are fired using the `SPACE` key
- Become inactive after leaving the screen
- Become inactive after hitting an enemy
- Can be reused after becoming inactive

The project uses **3 bullet slots** for active/inactive bullet management.

---

## 👾 Enemy System

Enemies are stored using arrays for their X and Y positions.

Enemy behavior includes:

- Multiple enemies on screen
- Random horizontal positions
- Downward movement
- Respawning after being destroyed
- Respawning after reaching the bottom
- Increasing movement speed as the game progresses

Enemies are represented using simple 2D shapes drawn with OpenGL.

---

## 💥 Collision Detection

### Bullet vs Enemy

When a bullet hits an enemy:

1. The bullet becomes inactive.
2. The enemy is respawned at a new position.
3. The player's score increases.

Each destroyed enemy awards **10 points**.

### Enemy vs Player

When an enemy collides with the player:

- The player loses a life.
- The enemy is respawned.

### Enemy vs Bottom

When an enemy reaches the bottom of the screen:

- The player loses one life.
- The enemy is moved back to the top.
- A new random X position is generated.

---

## 📈 Scoring and Difficulty

The player starts with:

```text
Score = 0
Lives = 3
