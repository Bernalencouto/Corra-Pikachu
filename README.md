# ⚡ CORRA, PIKACHU!

## 📖 Sobre o Jogo

**CORRA, PIKACHU!** é um jogo desenvolvido em C utilizando a biblioteca gráfica **Raylib**. O objetivo é ajudar o Pikachu a correr por um cenário desafiador, desviando de Pokébolas e móveis para encontrar seu treinador, Ash. Teste seus reflexos no Modo História e desbloqueie o desafio supremo no Modo Infinito!

## 👥 Membros

* **Bernardo Alencar Coutinho** - bac2@cesar.school 📩
* **Breno Pereira de Oliveira Lima** - bpol@cesar.school 📩
* **João Carlos Vasconcelos de Gusmão** - jcvg@cesar.school 📩

## ⚙️ Requisitos do Sistema

* **Linux** (Ubuntu/Debian), **macOS** ou **Windows**
* Compilador **GCC**
* **Make** (ou `mingw32-make` no Windows)
* Biblioteca gráfica **Raylib**

## 💾 Instalação e Dependências

Este projeto utiliza a biblioteca **Raylib**. Siga as instruções abaixo para preparar o seu ambiente antes de compilar.

### 🐧 Linux (Ubuntu/Debian)

1.  Instale as dependências de compilação e as bibliotecas gráficas/áudio necessárias:
    ```bash
    sudo apt update
    sudo apt install build-essential git libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev libraylib-dev
    ```

### 🍎 macOS

1.  Instale o Homebrew (caso ainda não tenha):
    ```bash
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    ```
2.  Instale a Raylib via Homebrew:
    ```bash
    brew install raylib
    ```

### 🪟 Windows

1.  Recomenda-se o uso do **MinGW** e a instalação da Raylib via **vcpkg**.
2.  Configure a Variável de Ambiente `VCPKG_ROOT` para a pasta de instalação do vcpkg.
3.  **Nota Importante:** Se o comando `make` não for reconhecido, configure o alias para o MinGW executando este comando no PowerShell antes de compilar:
    ```powershell
    Set-Alias -Name make -Value mingw32-make
    ```

## 🚀 Executando o Jogo

Para baixar e rodar o jogo, siga estas etapas no seu terminal:

1.  **Clone este repositório em sua máquina:**
    ```bash
    git clone https://github.com/Bernalencouto/Corra-Pikachu.git
    ```

2.  **Compile e execute o programa:**
    Entre na pasta e use o comando `make run` (que compila e abre o jogo automaticamente):
    ```bash
    cd Corra-Pikachu
    make run
    ```

## 🎮 Guia de Jogo

### ⌨️ Navegação no Menu

* Use as **setas ↑ e ↓** (ou **W** e **S**) para navegar pelas opções.
* Use as **setas ← e →** (ou **A** e **D**) para ajustar o volume.
* Pressione **Enter** para selecionar.

### 📋 Opções do Menu

* **Modo História:** Tente alcançar 4000 pontos para encontrar o Ash e vencer!
* **Modo Infinito:** Desbloqueado após vencer o Modo História. Teste quanto tempo consegue sobreviver.
* **Volume:** Ajusta o som do jogo.
* **Som ON/OFF:** Liga ou desliga o áudio.

### 🏃 Mecânicas 

* **Pular:** Pressione **Espaço**.
* **Pulo Duplo:** Pressione **Espaço** novamente no ar para um segundo pulo.
* **Objetivo:** Desvie das Pokébolas, Cadeiras e Mesas.
* **Game Over:** Acontece se tocar nas Pokébolas ou se for empurrado o suficiente pelos outros obstáculos.

### 🏆 Vitória / Game Over

* **Enter:** Retorna ao Menu Principal para tentar novamente.

---

## 🎥 Gameplay 

Confira o vídeo demonstrativo do jogo em funcionamento:
[**📺 Assistir no YouTube**](https://youtu.be/AUF6k52xBiA)

---
*Aproveite e tente bater o High Score!*