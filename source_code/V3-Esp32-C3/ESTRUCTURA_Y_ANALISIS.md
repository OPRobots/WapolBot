# WapolBot V3 — ESP32-C3 · Estructura, clases y análisis de `delay()`

> Plataforma: Seeed XIAO ESP32-C3  
> Framework: Arduino (PlatformIO)  
> Lib externa: Adafruit NeoPixel 1.12+

---

## 1. Arquitectura general

```
main.cpp
  ├── setup()
  │     ├── init_components()   ← pinout.cpp
  │     ├── pidSetup()          ← pid.cpp
  │     └── Serial.begin()
  └── loop()
        └── menu1()             ← menu.cpp
```

El loop principal **solo llama a `menu1()`**. Todo el comportamiento del robot se orquesta desde ahí o desde las estrategias.

---

## 2. Módulos / "clases" (archivos)

### 2.1 `pinout` — Inicialización de hardware

| Archivo | `include/pinout.h` · `src/pinout.cpp` |
|---|---|
| **Responsabilidad** | Definir pines y arrancar todos los subsistemas |
| **Función clave** | `init_components()` |

**Pines definidos:**

| Nombre | Pin | Tipo |
|---|---|---|
| `NEOPIXEL` | 20 | LED WS2812 (3 LEDs) |
| `MOD_START` | 21 | Entrada RC5 (interrupt) |
| `BTN_1` | 6 | Botón físico (INPUT_PULLDOWN) |
| `MOTOR_RIGHT_A/B` | 2, 8 | Motor derecho (PWM) |
| `MOTOR_LEFT_A/B` | 7, 10 | Motor izquierdo (PWM) |
| `S_RIVAL_I/C/D` | 1, 3, 4 | Sensores Sharp IR frontales (analog) |
| `S_LINEA_I/D` | 0, 5 | Sensores de línea QRE1113 (digital) |

`init_components()` llama a `init_motors()` y adjunta ISR al pin `MOD_START`.

---

### 2.2 `control` — Estado de carrera

| Archivo | `include/control.h` · `src/control.cpp` |
|---|---|
| **Responsabilidad** | Flag global `starting` (en carrera / en espera) |
| **Variables** | `static bool starting` |
| **Funciones** | `set_starting(bool)`, `is_starting()` |

Muy simple. Es la "fuente de verdad" que determina si el robot está activo o en menú.

---

### 2.3 `motors` — Control de motores DC

| Archivo | `include/motors.h` · `src/motors.cpp` |
|---|---|
| **Responsabilidad** | PWM de 4 canales para dos motores H-Bridge |
| **PWM** | 1 kHz, 10 bits (0–1023), canales 0–3 |
| **Variables globales** | `vel=60`, `velocidad_base=60`, `velD`, `velI` |

**Jerarquía de funciones:**

```
Nivel bajo (un motor):
  motorDerechoAdelante / Atras / Stop
  motorIzquierdoAdelante / Atras / Stop

Nivel medio (ambos motores):
  motoresAdelante / Atras / Stop
  motoresGiroDerecha / Izquierda          ← giro abierto (1 motor)
  motoresGiroDerechaCerrado / Izquierda   ← giro cerrado (2 motores opuestos)

Nivel alto (maniobras temporizadas con delay):  ⚠️
  motoresSusto()           — adelante 30 ms, stop
  motoresGirar45Derecha()  — cerrado 80 ms, stop
  motoresGirar45Izquierda()— cerrado 85 ms, stop
  motoresGirar90Derecha()  — cerrado 140 ms, stop
  motoresGirar90Izquierda()— cerrado 140 ms, stop
  motoresGirar180()        — cerrado izq 260 ms, stop
```

---

### 2.4 `sensors` — Sensores IR

| Archivo | `include/sensors.h` · `src/sensors.cpp` |
|---|---|
| **Responsabilidad** | Lectura y filtrado de Sharp IR (rival) y QRE1113 (línea) |

**Sensores frontales Sharp GP2Y0A (rival):**
- 3 sensores analógicos: Izquierdo (`S_RIVAL_I`), Centro (`S_RIVAL_C`), Derecho (`S_RIVAL_D`)
- Filtro de media móvil de 100 muestras por sensor (`filtro_sensores2()`)
- Conversión voltaje→distancia con funciones `voltageToDistanceIC()` / `voltageToDistanceD()`

**Umbrales de distancia:**
| Función | Rango (cm) | Significado |
|---|---|---|
| `sharpXDetectado()` | 17–29 cm | Rival en rango medio |
| `sharpXCerca()` | 0–17 cm | Rival muy cerca |

**Sensores de suelo QRE1113:**
- 2 sensores digitales: Izquierdo (`S_LINEA_I`), Derecho (`S_LINEA_D`)
- Filtro de media móvil de 10 muestras (`filtrarSensoresSueloI/D()`)
- `qre1113IzquierdoBlanco()`, `qre1113DerechoBlanco()`, `qre1113Blancos()`

---

### 2.5 `pid` — Control PID de seguimiento

| Archivo | `include/pid.h` · `src/pid.cpp` |
|---|---|
| **Responsabilidad** | PD (Proporcional-Derivativo) para orientar el robot hacia el rival |
| **Parámetros** | `kp=0.8`, `kd=1.0`, `tiempoPID=10 ms` |

**Variables relevantes:**
- `velocidad`, `getVelBase()=60`, `velocidadMaxima=100`
- `millisPID` — marca de tiempo para la cadencia del PID (no bloqueante ✅)
- `millisSusto` — cadencia para ejecutar `motoresSusto()` (no bloqueante ✅)
- `rivalDetectado`, `tiempoDeteccion`, `ultimoIncremento` — lógica de aceleración progresiva

**Flujo de `doPid()`:**
```
doPid()  [llamado cada 10 ms via millis]
  ├── proporcionalSimple()   ← lee sharpXCerca(), actualiza LEDs
  ├── calcula P + D = correccion
  ├── lógica de aceleración si rival al centro
  └── movimiento(pos, correccion, velI, velD)
        └── setVelI/D + motoresXxx()
```

`proporcionalSimple()` retorna valores: -100, -50, 0, 50, 100 (rival visto) · -10/10 (perdido, `lastSeen`).

---

### 2.6 `leds` — NeoPixel RGB

| Archivo | `include/leds.h` · `src/leds.cpp` |
|---|---|
| **Responsabilidad** | Control de 3 LEDs WS2812 (LEFT=2, TOP=1, RIGHT=0) |

**Funciones base:**
- `set_led(led, r,g,b)` / `clear_led(led)` / `blink_led(led, r,g,b, ms)` — no bloqueantes ✅
- `rainbow_led(led)` / `rainbow_leds()` — no bloqueante ✅

**Helpers de color:**
- `ledsUp/Left/Right/LeftRight(val)` — rojo ON/OFF por zona
- `ledsLoading(val)` — todos rojo ON/OFF
- `ledsGreen(val)` / `set_leds()` / `clear_leds()`

**Función problemática:**
- `ledsCuentaAtras()` — 5 × `delay(1000)` seguidos = **5 segundos bloqueantes** ⚠️ (actualmente comentada en menu.cpp)

---

### 2.7 `buttons` — Botón físico

| Archivo | `include/buttons.h` · `src/buttons.cpp` |
|---|---|
| **Responsabilidad** | Detectar pulsación corta / larga del `BTN_1` |

**Estados (`BTN_STATES`):**
```
BTN_RELEASED       — no pulsado
BTN_PRESSING       — pulsando (aún no soltado)
BTN_PRESSED        — pulsación corta (< 250 ms)
BTN_LONG_PRESSED   — pulsación larga (≥ 250 ms)
```

La detección ya usa `millis()` con un debounce de 50 ms. **No usa `delay()`** ✅ internamente.

---

### 2.8 `rc5` — Receptor IR RC5 (mando a distancia)

| Archivo | `include/rc5.h` · `src/rc5.cpp` |
|---|---|
| **Responsabilidad** | Decodificar protocolo RC5 via interrupción en `MOD_START` |

**Flujo:**
```
ISR: rc5_isr()
  └── rc5_register(FALLING/RISING)
        ├── mide tiempo con micros()
        ├── delayMicroseconds(100)   ⚠️ delay en ISR
        └── si bits==14 → rc5_manage_command()
              ├── ADDRESS_PROG  → guarda START/STOP codes + prog_done=true
              └── ADDRESS_COMP  → set_starting(true/false)
```

**Dato: `delayMicroseconds(100)` está dentro de `rc5_register()`**, que se llama desde la ISR. En ESP32 esto es arriesgado (ISRs deben ser mínimas).

---

### 2.9 `estrategias` — Estrategias de combate

| Archivo | `include/estrategias.h` · `src/estrategias.cpp` |
|---|---|
| **Responsabilidad** | Lógica de comportamiento del robot según modo seleccionado |

**Estrategias disponibles (seleccionadas por menú 1–9):**

| Pos | Estrategia | Descripción |
|---|---|---|
| 1 | *(base)* | `estrategiaBase()` — bucle principal de combate |
| 2 | `estrategiaMirarAtras()` | Giro 180° al arrancar |
| 3 | `estrategiaMirarLadoDerecha()` | Giro 90° derecha al arrancar |
| 4 | `estrategiaMirarLadoIzquierda()` | Giro 90° izquierda al arrancar |
| 5 | `estrategiaCaja()` | Avanza 300 ms (empuje inicial) ⚠️ |
| 6 | `estrategiaRadar()` | Solo PID sin control de velocidad (bucle infinito) |
| 7 | `estrategiaSharps()` | Debug de sensores Sharp (bucle infinito) |
| 8 | `estrategiaQre1113()` | Debug de sensores de suelo (bucle infinito) |
| 9 | `estrategiaMotores()` | Debug de motores con botón (bucle infinito) |

**`estrategiaBase()` — flujo principal de combate:**
```
estrategiaBase()
  ├── si línea blanca detectada:
  │     ├── stop
  │     ├── si ambas: atrás 80 ms ⚠️ + giro 180° ⚠️
  │     ├── si izq blanca: giro 45° izq ⚠️
  │     └── si der blanca: giro 45° der ⚠️
  └── si no hay línea:
        ├── filtro_sensores2()
        └── doPid()
```

---

### 2.10 `menu` — Menú de selección

| Archivo | `include/menu.h` · `src/menu.cpp` |
|---|---|
| **Responsabilidad** | Interfaz de usuario: selección de estrategia con botón y arranque |

**Flujo de `menu1()`:**
```
menu1()
  ├── [inMenu=true]
  │     ├── muestra LEDs según position (1–9)
  │     ├── BTN_PRESSED → position++, delay(200) ⚠️
  │     ├── rc5_is_prog_done() → blink 1 segundo (bucle bloqueante) ⚠️
  │     └── BTN_LONG_PRESSED / is_starting()
  │           └── llama estrategia según position → inMenu=false
  └── [inMenu=false]
        ├── si !is_starting(): stop + set_starting(false)
        └── si is_starting(): estrategiaBase()
```

---

## 3. Inventario completo de `delay()` en el proyecto

| # | Archivo | Función | `delay` / bloqueo | Duración | Propósito |
|---|---|---|---|---|---|
| 1 | `src/menu.cpp` | `menu1()` | `delay(200)` | 200 ms | Evitar doble detección al cambiar posición |
| 2 | `src/menu.cpp` | `menu1()` | `while(millis()-ms < 1000)` | 1000 ms | Feedback LED cuando RC5 programa OK |
| 3 | `src/motors.cpp` | `motoresSusto()` | `delay(30)` | 30 ms | Avance brusco de 30 ms |
| 4 | `src/motors.cpp` | `motoresGirar45Izquierda()` | `delay(85)` | 85 ms | Giro 45° izq por tiempo |
| 5 | `src/motors.cpp` | `motoresGirar45Derecha()` | `delay(80)` | 80 ms | Giro 45° der por tiempo |
| 6 | `src/motors.cpp` | `motoresGirar90Izquierda()` | `delay(140)` | 140 ms | Giro 90° izq por tiempo |
| 7 | `src/motors.cpp` | `motoresGirar90Derecha()` | `delay(140)` | 140 ms | Giro 90° der por tiempo |
| 8 | `src/motors.cpp` | `motoresGirar180()` | `delay(260)` | 260 ms | Giro 180° por tiempo |
| 9 | `src/estrategias.cpp` | `estrategiaSetup()` | `delay(3000)` | 3000 ms | Espera inicial (no se usa actualmente) |
| 10 | `src/estrategias.cpp` | `estrategiaBase()` | `delay(80)` (via motoresAtras+delay) | 80 ms | Retroceso al detectar línea |
| 11 | `src/estrategias.cpp` | `estrategiaCaja()` | `delay(300)` | 300 ms | Avance 300 ms inicial |
| 12 | `src/estrategias.cpp` | `estrategiaSharps()` | `delay(200)` | 200 ms | Debounce botón en debug |
| 13 | `src/estrategias.cpp` | `estrategiaMotores()` | `delay(200)` | 200 ms | Debounce botón en debug |
| 14 | `src/leds.cpp` | `ledsCuentaAtras()` | 5 × `delay(1000)` | 5000 ms | Animación cuenta atrás (comentada) |
| 15 | `src/sensors.cpp` | `print_sensores_1()` | `delay(500)` | 500 ms | Throttle de debug serial |
| 16 | `src/sensors.cpp` | `print_sensores_2()` | `delay(500)` | 500 ms | Throttle de debug serial |
| 17 | `src/rc5.cpp` | `rc5_register()` | `delayMicroseconds(100)` | 0.1 ms | Pausa en ISR (problemático) |

**Total: 17 puntos de bloqueo identificados.**

---

## 4. Clasificación por impacto

### 🔴 CRÍTICOS — Bloquean el loop durante la carrera

Estos delays ocurren dentro del flujo de combate activo (`estrategiaBase`). Durante su ejecución el robot no puede leer sensores ni reaccionar:

| # | Contexto | Duración bloqueada |
|---|---|---|
| 3 | `motoresSusto()` (llamado desde `doPid()`) | 30 ms |
| 4–8 | Giros 45°/90°/180° (llamados desde `estrategiaBase`) | 80–260 ms |
| 10 | Retroceso + giro en `estrategiaBase` al ver línea blanca | 80 + 85/140/260 ms |

### 🟡 MODERADOS — Bloquean en transición de estado

| # | Contexto | Duración bloqueada |
|---|---|---|
| 1 | Cambio de posición en menú | 200 ms |
| 2 | Feedback RC5 en menú | 1000 ms (pero usa `blink_led` dentro) |
| 11 | `estrategiaCaja()` — avance inicial | 300 ms |

### 🟢 BAJO IMPACTO — Solo en modo debug / nunca en combate

| # | Contexto | Duración bloqueada |
|---|---|---|
| 9 | `estrategiaSetup()` — no se llama desde ningún sitio | 3000 ms |
| 12–13 | Debounce botón en estrategias de debug | 200 ms |
| 14 | `ledsCuentaAtras()` — comentada | 5000 ms |
| 15–16 | `print_sensores_X()` — debug | 500 ms |
| 17 | `delayMicroseconds(100)` en ISR RC5 | 0.1 ms |

---

## 5. Plan para eliminar los delays

### Patrón general: máquina de estados + `millis()`

El patrón para reemplazar `funcionGiro(); delay(X); motorStop();` es una máquina de estados no bloqueante:

```cpp
// En lugar de:
void motoresGirar90Derecha() {
    motoresGiroDerechaCerrado();
    delay(140);
    motoresStop();
}

// Se usa un estado + timestamp:
enum MANIOBRA_STATE { MANIOBRA_IDLE, MANIOBRA_GIRANDO };

struct Maniobra {
    MANIOBRA_STATE state = MANIOBRA_IDLE;
    unsigned long startMs = 0;
    unsigned long duration = 0;
};
```

Cada maniobra que antes era bloqueante pasa a tener 3 fases:
1. **inicio** — activa motores, guarda `millis()` y duración
2. **update** — se llama en cada iteración del loop; si `millis()-start >= duration` → para motores
3. **fin** — retorna `true` o pasa al siguiente estado

---

### 5.1 Reemplazos urgentes (críticos en combate)

#### `motors.cpp` — Maniobras temporizadas

Convertir las funciones `motoresGirarXxx()` y `motoresSusto()` en funciones de **inicio + polling**:

```cpp
// Nueva API propuesta:
bool motoresGirar90Derecha();   // retorna true cuando terminó
bool motoresSusto();            // retorna true cuando terminó
```

Internamente usan `millis()` con una `static unsigned long startMs` y un flag de activo.

#### `estrategias.cpp` — `estrategiaBase()`

El bloque de reacción a línea blanca actualmente:
```cpp
motoresAtras();
delay(80);           // ← bloquea
motoresGirar180();   // ← bloquea 260 ms más
```

Debe convertirse en una **FSM (Finite State Machine)**:

```
Estado LIBRE        → leer sensores → doPid()
Estado RETROCESO    → atrás durante 80 ms → pasar a GIRO
Estado GIRO_45_IZQ  → giro cerrado durante 85 ms → pasar a LIBRE
Estado GIRO_45_DER  → giro cerrado durante 80 ms → pasar a LIBRE
Estado GIRO_180     → giro cerrado durante 260 ms → pasar a LIBRE
```

Cada transición se dispara cuando `millis() - estadoStartMs >= duracion`.

---

### 5.2 `menu.cpp` — `delay(200)` en cambio de posición

Ya existe debounce en `buttons.cpp` (`btn_last_pressed_ms` con 50 ms). El `delay(200)` en el menú es redundante pero añade margen extra.

**Solución:** Añadir una variable `static unsigned long lastPositionChange = 0` y comprobar `millis() - lastPositionChange > 200` antes de procesar `BTN_PRESSED`.

---

### 5.3 `menu.cpp` — bucle bloqueante RC5

```cpp
// Actual (bloqueante):
long ms = millis();
while(millis()-ms < 1000){
    blink_led(RGB_TOP, 0, 0, 0, 50);
}

// Propuesta (no bloqueante):
// Guardar rc5FeedbackStart = millis() cuando prog_done
// En cada loop: blink_led(...) mientras millis()-rc5FeedbackStart < 1000
// Luego rc5_reset_prog_done()
```

---

### 5.4 `rc5.cpp` — `delayMicroseconds(100)` en ISR

Este delay está **dentro de la función `rc5_register()`** que se llama desde la ISR `rc5_isr()`. En ESP32-C3 las ISRs deben ser lo más rápidas posible.

**Propuesta:** Eliminar el `delayMicroseconds(100)` directamente. Su propósito probable era un pequeño debounce de la señal IR, pero el protocolo RC5 ya maneja los tiempos por el análisis de pulsos (`MIN_SHORT`, `MAX_SHORT`). Quitarlo no debería afectar la decodificación.

---

### 5.5 `leds.cpp` — `ledsCuentaAtras()`

Actualmente comentada en menu.cpp. Si se quiere reactivar, implementarla como secuencia no bloqueante con `millis()` y un índice de paso.

---

### 5.6 `sensors.cpp` — `print_sensores_X()`

Solo son funciones de debug. Reemplazar `delay(500)` por:
```cpp
static unsigned long lastPrint = 0;
if(millis() - lastPrint >= 500) {
    // imprimir...
    lastPrint = millis();
}
```

---

## 6. Orden de implementación recomendado

| Prioridad | Tarea | Riesgo de romper comportamiento |
|---|---|---|
| 1 | Eliminar `delay(200)` en `menu1()` (cambio posición) | Muy bajo — ya hay debounce |
| 2 | Eliminar `delayMicroseconds(100)` en `rc5_register()` | Bajo — RC5 ya maneja tiempos |
| 3 | Reemplazar `delay(500)` en `print_sensores_X()` | Nulo — solo debug |
| 4 | Reemplazar bucle bloqueante RC5 en `menu1()` | Bajo — solo estética LED |
| 5 | FSM para `estrategiaBase()` (reacción a línea) | **Alto** — núcleo del combate |
| 6 | FSM para `motoresGirarXxx()` y `motoresSusto()` | **Alto** — cambia API de motores |
| 7 | FSM para `estrategiaCaja()` | Medio |
| 8 | Reactivar y reimplementar `ledsCuentaAtras()` | Bajo — opcional |

---

## 7. Notas sobre el diseño actual

- El PID (`doPid()`) y el blink de LEDs ya son **100% no bloqueantes** — usan `millis()` correctamente. Son el patrón a seguir.
- Los `while(true)` en estrategias de debug (Radar, Sharps, Qre1113, Motores) son bucles infinitos intencionados — el robot sale de ellos solo por reset/RC5. No son un problema per se, pero bloquean el loop de Arduino indefinidamente.
- `motoresSusto()` se llama desde `doPid()` cada `tiempoSusto=1000 ms` cuando `posicion == ±10` (rival perdido). Convertirlo a no bloqueante es importante para no perder ciclos de sensor durante ese susto.
- Las funciones `motoresGirarXxx()` son usadas tanto en estrategias de inicio (posiciones 2–4, solo una vez al arrancar) como en `estrategiaBase()` (repetidas en combate). Las del inicio son menos urgentes de convertir; las de combate son críticas.
