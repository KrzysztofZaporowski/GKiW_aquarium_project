# Aquarium 3D - OpenGL Project

## 📖 Opis Projektu

![Image](https://github.com/user-attachments/assets/e4ad1f6e-b057-4e3e-9ce5-940bef7bbd69)
![Image](https://github.com/user-attachments/assets/aa3559d3-3f25-4121-9092-f7ab6ff5b91a)

Interaktywne 3D akwarium z rybami, koralami i kamieniami, renderowane w czasie rzeczywistym przy użyciu OpenGL. Projekt symuluje podwodne środowisko z realistycznymi efektami wizualnymi, animacjami i systemem oświetlenia.

### Główne Funkcje
- 🐠 **Animowane ryby** z AI ruchu (pływają losowo po akwarium)
- 🪨 **Kamienie** z proceduralnym rozmieszczeniem 
- 🌿 **Koralle** z animacją kołysania
- 💧 **Efekty wodne** (caustics, mgła, shimmer)
- 🏠 **Środowisko akwarium** (szkło, rama, podłoga piaskowa)
- 🌅 **Dynamiczny skybox** (podwodny vs pokój)
- 🎮 **Sterowanie kamerą** (WSAD + mysz)

---

## 🎮 Sterowanie Kamerą

| Klawisz/Akcja | Funkcja |
|---------------|---------|
| **W** | Ruch do przodu |
| **S** | Ruch do tyłu |
| **A** | Ruch w lewo |
| **D** | Ruch w prawo |
| **Mysz** | Obracanie widoku (przytrzymaj **lewy przycisk myszy**) |
| **Scroll** | Zoom (zmiana FOV: 1-45°) |

### Ograniczenia Kamery
- **Wysokość**: -0.8 do 8.0 jednostek
- **Automatyczne przełączanie skybox**: podwodny (wewnątrz akwarium) vs pokój (na zewnątrz)
- **Prędkość**: 2.5 jednostek/sekundę
- **Czułość myszy**: 0.1 (konfigurowalne w kodzie)

---

## 📁 Struktura Projektu

### 🔧 Pliki Główne
| Plik | Opis |
|------|------|
| `main_file.cpp` | **Główny plik aplikacji** - pętla renderowania, inicjalizacja OpenGL, zarządzanie kamerą |
| `shaderprogram.h/.cpp` | **Klasa ShaderProgram** - ładowanie, kompilacja i zarządzanie shaderami |
| `vertices.h/.cpp` | **Geometrie podstawowe** - definicje wierzchołków dla szkła, ramy, podłóg, skybox |

### 🎨 Klasy Obiektów
| Plik | Opis |
|------|------|
| `ModelLoader.h/.cpp` | **Ładowanie modeli 3D** - parser OBJ, ładowanie tekstur PNG, setup bufferów OpenGL |
| `Stone.h/.cpp` | **Klasa kamieni** - losowe rozmieszczenie, rendering z teksturami |
| `Fish.h/.cpp` | **Klasa ryb** - AI ruchu, animacje, collision detection z granicami akwarium |
| `Coral.h/.cpp` | **Klasa korali** - animacje kołysania, unikanie kolizji z kamieniami |

### 🖼️ Biblioteki
| Plik | Opis |
|------|------|
| `lodepng.h/.cpp` | **Biblioteka PNG** - ładowanie tekstur z plików PNG |
| `constants.h` | **Stałe matematyczne** - definicja PI |

### 🎭 Shadery - Vertex (Geometria)
| Plik | Opis |
|------|------|
| `v_skybox.glsl` | Shader wierzchołków dla podwodnego skybox |
| `v_room_skybox.glsl` | Shader wierzchołków dla skybox pokoju |
| `v_sand.glsl` | Shader wierzchołków dla podłogi piaskowej |
| `v_glass.glsl` | Shader wierzchołków dla szklanych ścian |
| `v_frame.glsl` | Shader wierzchołków dla metalowej ramy |
| `v_outside.glsl` | Shader wierzchołków dla podłogi zewnętrznej |
| `v_water_fog.glsl` | Shader wierzchołków dla mgły wodnej |
| `v_stone.glsl` | Shader wierzchołków dla kamieni |
| `v_fish.glsl` | Shader wierzchołków dla ryb |
| `v_coral.glsl` | **Animowany** shader wierzchołków dla korali (kołysanie) |

### 🌈 Shadery - Fragment (Kolory i Oświetlenie)
| Plik | Oświetlenie | Opis |
|------|-------------|------|
| `f_skybox.glsl` | **Emisyjne** | Podwodny skybox z caustics, falami, animowanymi promieniami słońca |
| `f_room_skybox.glsl` | **Emisyjne** | Jasny pokój z gradientem ścian/sufitu |
| `f_sand.glsl` | **2 źródła** | Piasek z caustics, sparkles, underwater tint |
| `f_glass.glsl` | **Fresnel** | Transparentne szkło z dystrorsją i kropelkami |
| `f_frame.glsl` | **1 źródło** | Metalowa rama z caustics i korozją |
| `f_outside.glsl` | **2 źródła** | Jasna podłoga pokoju (bez mgły) |
| `f_water_fog.glsl` | **Volumetric** | 3D mgła z cząsteczkami i promieniami |
| `f_stone.glsl` | **2 źródła** | Identyczne z piaskiem (caustics, shimmer) |
| `f_fish.glsl` | **1 źródło** | Proste oświetlenie z fog effect |
| `f_coral.glsl` | **1 źródło** | Podobne do ryb, nieco jaśniejsze |

---

## 🎨 Jak Zmienić Kolory i Oświetlenie

### 🌊 Kolory Tła Podwodnego (`f_skybox.glsl`)
```glsl
// Linie 33-36 - Kolory wody na różnych głębokościach
vec3 deepWater = vec3(0.1, 0.2, 0.35);        // Głęboka woda (ciemny niebieski)
vec3 midWater = vec3(0.2, 0.35, 0.5);         // Średnia głębia  
vec3 shallowWater = vec3(0.3, 0.5, 0.7);      // Płytka woda
vec3 surfaceWater = vec3(0.4, 0.6, 0.8);      // Powierzchnia (jasny niebieski)
```

### 🏠 Kolory Tła Pokoju (`f_room_skybox.glsl`)
```glsl
// Linie 12-14 - Kolory pomieszczenia
vec3 wallColor = vec3(0.95, 0.95, 0.97);      // Jasno szare ściany
vec3 ceilingColor = vec3(1.0, 1.0, 1.0);      // Biały sufit  
vec3 floorColor = vec3(0.85, 0.85, 0.9);      // Ciemniejsza podłoga
```

### 💡 Oświetlenie - Źródła Światła

#### Podłoga Piaskowa (`f_sand.glsl`)
```glsl
// Linie 29-34 - Główne źródła światła
vec3 lightDir1 = normalize(vec3(0.3, 1.0, 0.2));     // KIERUNEK głównego światła
vec3 lightColor1 = vec3(0.9, 1.0, 1.2);              // KOLOR: niebiesko-białe

vec3 lightDir2 = normalize(vec3(-0.5, 0.3, 0.8));    // KIERUNEK drugiego światła  
vec3 lightColor2 = vec3(0.6, 0.8, 1.1);              // KOLOR: niebieskie

vec3 ambient = vec3(0.5, 0.6, 0.7);                  // AMBIENT: jasne niebieskie
```

#### Podłoga Zewnętrzna (`f_outside.glsl`)
```glsl
// Linie 29-34 - Jasne oświetlenie pokoju
vec3 lightDir1 = normalize(vec3(0.5, 1.0, 0.3));     // Główne światło sufitowe
vec3 lightColor1 = vec3(1.2, 1.15, 1.1);             // Ciepłe białe światło

vec3 lightDir2 = normalize(vec3(-0.3, 0.8, -0.6));   // Drugie światło
vec3 lightColor2 = vec3(1.0, 1.05, 1.2);             // Chłodne światło

vec3 ambient = vec3(0.7, 0.7, 0.75);                 // Bardzo jasne ambient
```

#### Ryby (`f_fish.glsl`)
```glsl
// Linia 16 - Pojedyncze źródło światła
vec3 lightDir = normalize(vec3(0.2, 1.0, 0.3));      // Kierunek światła z góry
vec3 ambient = vec3(0.3, 0.4, 0.5);                  // Ambient podwodny
```

### 🔧 Jak Wyłączyć Źródło Światła

#### Przykład - Wyłączenie drugiego światła w `f_sand.glsl`:
```glsl
// PRZED (linia 45):
vec3 diffuse = (diff1 * lightColor1 * 0.7) + (diff2 * lightColor2 * 0.4);

// PO (wyłączenie drugiego światła):
vec3 diffuse = (diff1 * lightColor1 * 0.7); // + (diff2 * lightColor2 * 0.4);
```

#### Wyłączenie wszystkich efektów specjalnych w `f_sand.glsl`:
```glsl
// Wyłącz caustics (linia 69):
// finalColor += vec3(caustics * 1.2, caustics * 1.1, caustics);

// Wyłącz sparkles (linia 70):  
// finalColor += vec3(sparkle * 0.9, sparkle, sparkle * 1.1);

// Wyłącz shimmer (linia 77):
// finalColor += vec3(shimmer * 0.5, shimmer * 0.7, shimmer * 1.0);
```

### 🌊 Zmiana Intensywności Efektów Wodnych

#### Caustics (animowane wzory światła pod wodą):
```glsl
// f_skybox.glsl linia 86 - Zmień 0.2 na mniejszą wartość:
float caustics = max(caustics, 0.0) * 0.1; // Słabsze caustics
```

#### Mgła Wodna (`f_water_fog.glsl`):
```glsl
// Linia 56 - Zmień intensywność mgły:
fogDensity = clamp(fogDensity * 0.1 + particles + lightRays, 0.0, 0.2); // Słabsza mgła
```

### 🎯 Szybkie Modyfikacje

#### 1. **Jaśniejsze Akwarium** - Zwiększ ambient we wszystkich shaderach:
```glsl
vec3 ambient = vec3(0.8, 0.9, 1.0); // Zamiast vec3(0.5, 0.6, 0.7)
```

#### 2. **Bardziej Zielone Akwarium** - Zmień tint w `f_sand.glsl`:
```glsl
vec3 underwaterTint = vec3(0.85, 1.05, 0.95); // Więcej zieleni
```

#### 3. **Wyłącz Mgłę Wodną** - W `main_file.cpp` zakomentuj linie 308-320:
```cpp
// === DRAW WATER FOG ===
// if (insideAquarium) {
//     // ... cały blok rysowania mgły
// }
```

---

## 🛠️ Kompilacja

### Wymagane Biblioteki:
- **OpenGL 3.3+**
- **GLFW** (zarządzanie oknem)
- **GLEW** (rozszerzenia OpenGL)  
- **GLM** (matematyka 3D)

### Kompilacja (Linux/macOS):
```bash
g++ -std=c++17 main_file.cpp shaderprogram.cpp vertices.cpp lodepng.cpp \
    ModelLoader.cpp Stone.cpp Fish.cpp Coral.cpp \
    -lGL -lGLEW -lglfw -o aquarium
```

### Kompilacja (Windows - MinGW):
```bash
g++ -std=c++17 main_file.cpp shaderprogram.cpp vertices.cpp lodepng.cpp \
    ModelLoader.cpp Stone.cpp Fish.cpp Coral.cpp \
    -lopengl32 -lglfw3 -lglew32 -o aquarium.exe
```

---

## 🎪 Wymiary i Parametry Akwarium

| Element | Wymiary/Parametry |
|---------|-------------------|
| **Akwarium** | 16×5×12 jednostek (X: -8 do 8, Y: -1 do 4, Z: -6 do 6) |
| **Kamienie** | 6 sztuk (Rock01, Rock02), skala 0.3-0.8, min. 2 jednostki odstępu |
| **Koralle** | 3-5 sztuk, skala 0.4-0.8, animacja kołysania |
| **Ryby** | 8-12 sztuk (5 gatunków), skala 0.15-0.25, AI ruchu |
| **Szkło** | Przezroczystość 15-35%, efekt Fresnel |
| **Rama** | Linie 8.0f grubości, metaliczny materiał |

---

## Źródła
Modele 3D: https://free3d.com/pl/ 
Tekstury: https://unsplash.com/de 

## 📝 Autorzy
Projekt 3D Aquarium - OpenGL/C++

Aleksander Staszweski

Krzysztof Zaporowski

## 📄 Licencja

Cały kod w tym repozytorium jest open source i może być używany do własnych celów, również komercyjnych, zgodnie z poniższymi licencjami:

- **LodePNG** – objęty licencją **zlib**, autor: *Lode Vandevenne*
- **Pozostały kod** – objęty licencją **MIT**

### ✅ Co wolno:
- Używać kodu w projektach prywatnych i komercyjnych.
- Modyfikować kod, adaptować go do własnych potrzeb.
- Rozpowszechniać kod źródłowy lub binarny (np. jako część aplikacji).

### ⚠️ Warunki:
- Musisz zachować informacje o licencjach w kodzie (w plikach źródłowych lub dokumentacji).
- Nie wolno usuwać ani zmieniać oryginalnych adnotacji licencyjnych.
- W przypadku modyfikacji **LodePNG**, należy zaznaczyć, że kod został zmieniony (zgodnie z licencją **zlib**).

