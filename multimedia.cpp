#include "multimedia.h"
#include <iostream>

using namespace std;
#ifdef CON_OPENCV
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

// OpenCV llama a esta funcion cuando ocurre un evento del mouse.
void cerrarPortadaConClic(int evento, int, int, int, void* datos) {
    if (evento == cv::EVENT_LBUTTONUP) {
        bool* cerrar = static_cast<bool*>(datos);
        *cerrar = true;
    }
}

void mostrarPortada(const string& ruta) {
    try {
        cv::Mat imagen = cv::imread(ruta);
        if (imagen.empty()) { cout << "No se pudo abrir la portada: " << ruta << '\n'; return; }
        const string ventana = "Portada - clic en la imagen o Esc para volver";
        bool cerrar = false;
        cout << "Haz clic en la imagen o presiona Esc en su ventana para volver al menu." << endl;
        cv::imshow(ventana, imagen);
        cv::setMouseCallback(ventana, cerrarPortadaConClic, &cerrar);
        while (!cerrar) {
            if (cv::waitKey(30) >= 0) break;
            if (cv::getWindowProperty(ventana, cv::WND_PROP_VISIBLE) < 1) break;
        }
        cv::destroyAllWindows();
        cv::waitKey(1); // Procesa el cierre de la ventana antes de volver al menu.
    } catch (const cv::Exception& e) {
        cv::destroyAllWindows();
        cout << "No se pudo mostrar la imagen: " << e.what() << '\n';
    }
}
#else
void mostrarPortada(const string&) {
    cout << "Esta compilacion es solo de consola. Ejecuta ./compilar.sh grafico.\n";
}
#endif
