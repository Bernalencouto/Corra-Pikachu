# ⚡ CORRA, PIKACHU!

## 📄 Descrição

**CORRA, PIKACHU!** é um jogo desenvolvido em C utilizando a biblioteca gráfica **Raylib**. O objetivo é ajudar o Pikachu a correr por um cenário desafiador, desviando de Pokébolas e móveis para encontrar seu treinador, Ash. Teste seus reflexos no Modo História e desbloqueie o desafio supremo no Modo Infinito!

## 👩‍💻 Membros

* **Bernardo Alencar Coutinho** - bac2@cesar.school 📩
* **Breno Pereira de Oliveira Lima** - bpol@cesar.school 📩
* **João Carlos Vasconcelos de Gusmão** - jcvg@cesar.school 📩

## ✅ Pré-requisitos

* **Linux** (Ubuntu/Debian), **macOS** ou **Windows**
* Compilador **GCC**
* **Make**
* Biblioteca gráfica **Raylib**

## 🛠️ Instalação da raylib

Este projeto utiliza a biblioteca **Raylib**. Siga as instruções abaixo para preparar o seu ambiente.

### 🐧 Linux (Ubuntu/Debian)

1.  Instale as dependências de compilação e a Raylib:
    ```bash
    sudo apt update
    sudo apt install build-essential git libraylib-dev
    ```

### 🍎 macOS

1.  Instale o Homebrew (caso ainda não tenha):
    ```bash
    /bin/bash -c "$(curl -fsSL [https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh](https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh))"
    ```
2.  Instale a Raylib via Homebrew:
    ```bash
    brew install raylib
    ```

### 🪟 Windows

1.  Recomenda-se o uso do **MinGW** e a instalação da Raylib via **vcpkg**.
2.  Configure a Variável de Ambiente `VCPKG_ROOT` para a pasta de instalação do vcpkg.

## ♟️ Executando o Jogo

Para baixar e rodar o jogo, siga estas etapas no seu terminal:

1.  **Clone este repositório em sua máquina:**
    ```bash
    git clone [https://github.com/Bernalencouto/Corra-Pikachu.git](https://github.com/Bernalencouto/Corra-Pikachu.git)
    ```

2.  **Compile e execute o programa:**
    Entre na pasta e use o comando `make run` (que compila e abre o jogo):
    ```bash
    cd Corra-Pikachu
    make run
    ```

Divirta-se jogando!

## 🕹️ Instruções do Jogo

### 🎮 Navegação no Menu

* Use as **setas ↑ e ↓** (ou **W** e **S**) para navegar pelas opções.
* Use as **setas ← e →** (ou **A** e **D**) para ajustar o volume.
* Pressione **Enter** para selecionar.

### 1️⃣ Opções do Menu

* **Modo História:** Tente alcançar 4000 pontos para encontrar o Ash e vencer!
* **Modo Infinito:** Desbloqueado após vencer o Modo História. Teste quanto tempo consegue sobreviver.
* **Volume:** Ajusta o som do jogo.
* **Som ON/OFF:** Liga ou desliga o áudio.

### 2️⃣ Durante o Jogo (Gameplay)

* **Pular:** Pressione **Espaço**.
* **Pulo Duplo:** Pressione **Espaço** novamente no ar para um segundo pulo.
* **Objetivo:** Desvie das Pokébolas, Cadeiras e Mesas.
* **Game Over:** Acontece se tocar nas Pokébolas ou se for empurrado o suficiente pelos outros obstáculos.

### 3️⃣ Game Over / Vitória

* **Enter:** Retorna ao Menu Principal para tentar novamente.

---
*Aproveite e tente bater o Hi-Score!*
