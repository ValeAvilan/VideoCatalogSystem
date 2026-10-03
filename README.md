# Video Catalog System

A small C++ application for managing a movie and TV series catalog. This project demonstrates object-oriented programming through a terminal menu and separate OpenCV windows for cover images.

The sample catalog includes **5 movies, 3 series, and 3 episodes per series**. The application interface and sample titles remain in Spanish.

## Features

- Load movies, series, episodes, and initial ratings from `datos.csv`.
- Browse all titles, only series, or only movies.
- Filter results by genre and minimum average rating.
- List the episodes of a selected series.
- Rate movies, entire series, and individual episodes with integer scores from 1 to 5.
- Display movie and series covers in OpenCV windows.
- Add and compare durations using overloaded operators in `Tiempo`.

Ratings added through the menu are stored **in memory for the current session**. Closing the application or reloading the CSV resets them to the initial file values. The program does not write ratings back to the CSV.

## Requirements

- A C++17 compiler; the build scripts use `clang++` by default.
- Bash to run the supplied scripts.
- OpenCV 4 or 5 and `pkg-config` for the image-enabled version.

The project has been compiled on macOS with OpenCV 5.0.0. The console version does not require OpenCV. Other platforms have not been verified.

## Build and run

Open a terminal in the project directory. If needed on macOS, install the compiler tools:

```bash
xcode-select --install
```

### Console version

```bash
bash compilar.sh consola
./build/catalogo_consola
```

This version provides the catalog menu and rating features. Requesting a cover displays a message that image support is unavailable.

### Version with cover images

On macOS with Homebrew installed, install the image dependencies:

```bash
brew install opencv pkg-config
```

Then compile and run:

```bash
bash compilar.sh grafico
./build/catalogo
```

To select another compiler, set `CXX`, for example:

```bash
CXX=g++ bash compilar.sh consola
```

Run the application **from the project directory**, because the CSV and image paths are relative to that directory.

## How to use the menu

Start with option **1** to load `datos.csv`. A successful load prints `Datos cargados correctamente.`

| Option | Action |
|---|---|
| 1 | Load or reload the sample catalog. |
| 2 | List movies and series, with optional genre and rating filters. |
| 3 | List only series, with the same filters. |
| 4 | List episodes of a series, filtered by minimum rating. |
| 5 | List only movies, with genre and rating filters. |
| 6 | Add a rating to a movie, series, or episode by title. |
| 0 | Exit. |

For browsing options, enter `0` as the minimum rating to include all ratings. Press Enter at the genre prompt to include all genres, or enter `Accion`, `Romance`, `Comedia`, `Drama`, or `Misterio` without accents. Genre matching ignores letter case.

For example, choose option **5**, enter minimum `3`, and enter `Accion` to list action movies with an actual average of at least 3.

After listing titles, enter a movie or series title to view its cover, or press Enter to return to the menu. Movie covers open directly; series covers ask for confirmation (`s` means yes). Option 4 also offers the selected series cover. Episodes do not have individual covers.

**Click the cover image or press Esc while its window is focused to close it and return to the terminal menu.** The terminal waits while a cover is open.

To rate a whole series, choose option **6**, enter a title such as `Friends`, and enter a score from 1 to 5. Movie and series title matching ignores letter case; episode titles must match exactly.

## Ratings and durations

A movie or episode average is the sum of its scores divided by the number of scores. A series average includes both its direct scores and all scores given to its episodes, with equal weight for each individual score.

Displayed averages are rounded to the nearest half point and use two decimal places, such as `3.00`, `3.50`, and `4.00`. Filters use the actual average before display rounding. Unrated content shows `Sin calificaciones` and uses 0 internally for filtering.

A series duration is the sum of its episode durations. The `Tiempo` class overloads `+`, `<`, `==`, and `<<` to add, compare, and display durations.

## Catalog data and cover images

`datos.csv` is a simple text file with **12 semicolon-separated fields** per record:

```text
tipo;id;nombre;genero;minutos;serie_id;temporada;numero;anio;clasificacion;portada;notas
```

The field description in the actual file starts with `#`: it is a comment, not a required table header. Empty lines and lines starting with `#` are skipped.

| Field | Meaning |
|---|---|
| `tipo` | `P` for movie, `S` for series, `E` for episode. |
| `id` | Unique numeric ID across movies, series, and episodes. |
| `nombre`, `genero` | Title and genre. |
| `minutos` | Duration in minutes; series durations are calculated from episodes. |
| `serie_id` | Parent series ID for an episode. |
| `temporada`, `numero` | Episode season and number. |
| `anio`, `clasificacion` | Movie release year and classification. |
| `portada` | Relative image path for a movie or series. |
| `notas` | Initial integer ratings, separated by commas; may be empty. |

Example records:

```text
P;101;Iron Man;Accion;126;0;0;0;2008;B;imagenes/iron_man.jpeg;4,2,5,3,4
S;201;Friends;Comedia;0;0;0;0;0;;imagenes/friends.jpg;3,4
E;301;The Pilot;Comedia;22;201;1;1;0;;;4,5,3
```

Place each series before its episode records. Use unique IDs, matching series and episode genres, and scores between 1 and 5. Keep all 12 fields; non-applicable fields use 0 or an empty value as shown above. Empty rating entries such as the extra commas in `3,4,,,` are ignored.

This reader supports the specified simple format; it does not implement general CSV quoting. Do not put semicolons inside field values. When editing in Excel, preserve the semicolon-separated records when saving.

Place cover images in `imagenes/` and update the `portada` field to match the exact file name and extension. Reload with option 1 to read changes. Missing or unreadable images produce a message. 

A failed catalog load leaves the previously loaded catalog intact and reports the line where validation failed.

## Project structure

| Files | Responsibility |
|---|---|
| `main.cpp` | Application entry point. |
| `interfaz.h`, `interfaz.cpp` | Terminal menu, input, and requests to display covers. |
| `multimedia.h`, `multimedia.cpp` | OpenCV image display and window closing. |
| `catalogo.h`, `catalogo.cpp` | CSV loading, searches, filters, and rating requests. |
| `video.h`, `video.cpp` | Abstract base class for shared video data and behavior. |
| `pelicula.h`, `pelicula.cpp` | Movie-specific information. |
| `serie.h`, `serie.cpp` | Episode collection, combined ratings, and total duration. |
| `episodio.h`, `episodio.cpp` | Episode season and number. |
| `tiempo.h`, `tiempo.cpp` | Durations and overloaded operators. |
| `calificacion.h`, `calificacion.cpp` | Score validation, storage, and averages. |
| `datos.csv`, `imagenes/` | Sample catalog and cover images. |
| `compilar.sh` | Build either application version. |
| `tests/pruebas.cpp`, `probar.sh` | Automated checks for core behavior. |

The design demonstrates **encapsulation**, **abstraction**, **inheritance**, and **polymorphism**. `Pelicula`, `Serie`, and `Episodio` inherit from `Video`; a series contains its episodes through composition. Menu handling and OpenCV display are kept separate from the catalog classes.

## Run the tests

From the project directory:

```bash
bash probar.sh
```

The test script builds with AddressSanitizer and UndefinedBehaviorSanitizer. Checks cover duration arithmetic, rating validation, series averages, virtual dispatch, catalog loading, empty rating entries from Excel, and preservation of the previous catalog after a failed load. The checks use the bundled sample data.

