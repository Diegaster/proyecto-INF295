#include "../include/Graficador.h"

#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include <string>

namespace
{
    struct Color
    {
        unsigned char r;
        unsigned char g;
        unsigned char b;
    };

    constexpr int WIDTH  = 1400;
    constexpr int HEIGHT = 1400;

    //--------------------------------------------------
    // PALETAS DE COLORES POR DEPÓSITO
    //--------------------------------------------------
    const Color COLORES_DEPOSITOS[] =
    {
        {220, 20, 60},    // Depósito 0: Gama de Rojos (Carmesí)
        {30, 144, 255},   // Depósito 1: Gama de Azules (DodgerBlue)
        {46, 139, 87},    // Depósito 2: Gama de Verdes (SeaGreen)
        {255, 140, 0},    // Depósito 3: Gama de Naranjas
        {138, 43, 226},   // Depósito 4: Gama de Púrpuras
        {0, 191, 255},    // Depósito 5: Gama de Cyans / Celestes
        {218, 165, 32}    // Depósito 6: Gama de Dorados
    };

    constexpr int NUM_COLORES_BASE = sizeof(COLORES_DEPOSITOS) / sizeof(Color);

    // Función auxiliar para generar variaciones (tonos) de un color base
    Color generarTonoRuta(Color base, int indiceRuta)
    {
        if (indiceRuta == 0) return base;
        
        // Modificamos la intensidad de forma controlada (atenuación/oscurecimiento)
        float multiplicador = 1.0f - (0.18f * indiceRuta);
        if (multiplicador < 0.35f) multiplicador = 0.35f; // Límite para evitar que se vuelva negro absoluto

        return {
            static_cast<unsigned char>(base.r * multiplicador),
            static_cast<unsigned char>(base.g * multiplicador),
            static_cast<unsigned char>(base.b * multiplicador)
        };
    }

    //--------------------------------------------------
    // PIXEL (Optimizado pasándolo por valor)
    //--------------------------------------------------

    void setPixel(
        std::vector<unsigned char>& img,
        int x,
        int y,
        Color c
    )
    {
        if(x < 0 || x >= WIDTH)
            return;

        if(y < 0 || y >= HEIGHT)
            return;

        int idx =
            (y * WIDTH + x) * 3;

        img[idx + 0] = c.r;
        img[idx + 1] = c.g;
        img[idx + 2] = c.b;
    }

    //--------------------------------------------------
    // CIRCULO
    //--------------------------------------------------

    void drawCircle(
        std::vector<unsigned char>& img,
        int cx,
        int cy,
        int radius,
        Color c
    )
    {
        for(int dx = -radius;
            dx <= radius;
            dx++)
        {
            for(int dy = -radius;
                dy <= radius;
                dy++)
            {
                if(
                    dx * dx +
                    dy * dy
                    <=
                    radius * radius
                )
                {
                    setPixel(
                        img,
                        cx + dx,
                        cy + dy,
                        c
                    );
                }
            }
        }
    }

    //--------------------------------------------------
    // CUADRADO
    //--------------------------------------------------

    void drawSquare(
        std::vector<unsigned char>& img,
        int cx,
        int cy,
        int half,
        Color c
    )
    {
        for(int dx = -half;
            dx <= half;
            dx++)
        {
            for(int dy = -half;
                dy <= half;
                dy++)
            {
                setPixel(
                    img,
                    cx + dx,
                    cy + dy,
                    c
                );
            }
        }
    }

    //--------------------------------------------------
    // LINEA
    //--------------------------------------------------

    void drawLine(
        std::vector<unsigned char>& img,
        int x1,
        int y1,
        int x2,
        int y2,
        Color c
    )
    {
        int dx =
            std::abs(x2 - x1);

        int dy =
            std::abs(y2 - y1);

        int sx =
            x1 < x2 ? 1 : -1;

        int sy =
            y1 < y2 ? 1 : -1;

        int err =
            dx - dy;

        while(true)
        {
            setPixel(img, x1, y1, c);
            // Dibujamos un píxel adyacente para dar consistencia al grosor de la línea
            setPixel(img, x1 + 1, y1, c);

            if(
                x1 == x2 &&
                y1 == y2
            )
            {
                break;
            }

            int e2 =
                2 * err;

            if(e2 > -dy)
            {
                err -= dy;
                x1 += sx;
            }

            if(e2 < dx)
            {
                err += dx;
                y1 += sy;
            }
        }
    }

    //--------------------------------------------------
    // FUENTE BITMAP
    //--------------------------------------------------

    const std::unordered_map<
        char,
        std::vector<std::string>
    > FONT =
    {
        {'A',{"01110","10001","10001","11111","10001","10001","10001"}},
        {'C',{"01110","10001","10000","10000","10000","10001","01110"}},
        {'D',{"11110","10001","10001","10001","10001","10001","11110"}},
        {'E',{"11111","10000","10000","11110","10000","10000","11111"}},
        {'I',{"11111","00100","00100","00100","00100","00100","11111"}},
        {'L',{"10000","10000","10000","10000","10000","10000","11111"}},
        {'N',{"10001","11001","10101","10011","10001","10001","10001"}},
        {'O',{"01110","10001","10001","10001","10001","10001","01110"}},
        {'P',{"11110","10001","10001","11110","10000","10000","10000"}},
        {'R',{"11110","10001","10001","11110","10100","10010","10001"}},
        {'S',{"01111","10000","10000","01110","00001","00001","11110"}},
        {'T',{"11111","00100","00100","00100","00100","00100","00100"}},
        {'U',{"10001","10001","10001","10001","10001","10001","01110"}},
        {'X',{"10001","01010","00100","00100","00100","01010","10001"}},
        {'Y',{"10001","01010","00100","00100","00100","00100","00100"}},

        {'0',{"01110","10001","10011","10101","11001","10001","01110"}},
        {'1',{"00100","01100","00100","00100","00100","00100","01110"}},
        {'2',{"01110","10001","00001","00010","00100","01000","11111"}},
        {'3',{"11110","00001","00001","01110","00001","00001","11110"}},
        {'4',{"00010","00110","01010","10010","11111","00010","00010"}},
        {'5',{"11111","10000","10000","11110","00001","00001","11110"}},
        {'6',{"01110","10000","10000","11110","10001","10001","01110"}},
        {'7',{"11111","00001","00010","00100","01000","01000","01000"}},
        {'8',{"01110","10001","10001","01110","10001","10001","01110"}},
        {'9',{"01110","10001","10001","01111","00001","00001","01110"}},

        {' ',{"00000","00000","00000","00000","00000","00000","00000"}}
    };

    //--------------------------------------------------
    // CHAR
    //--------------------------------------------------

    void drawChar(
        std::vector<unsigned char>& img,
        int x,
        int y,
        char c,
        Color color
    )
    {
        auto it =
            FONT.find(c);

        if(it == FONT.end())
            return;

        const auto& glyph =
            it->second;

        for(int row = 0;
            row < 7;
            row++)
        {
            for(int col = 0;
                col < 5;
                col++)
            {
                if(
                    glyph[row][col]
                    ==
                    '1'
                )
                {
                    drawSquare(
                        img,
                        x + col * 2,
                        y + row * 2,
                        1,
                        color
                    );
                }
            }
        }
    }

    //--------------------------------------------------
    // TEXT
    //--------------------------------------------------

    void drawText(
        std::vector<unsigned char>& img,
        int x,
        int y,
        const std::string& text,
        Color color
    )
    {
        int offset = 0;

        for(char c : text)
        {
            drawChar(
                img,
                x + offset,
                y,
                std::toupper(c),
                color
            );

            offset += 14;
        }
    }
}

void Graficador::graficar(
    const std::string& archivo,
    const Instancia& instancia,
    const Solucion& solucion
)
{
    std::vector<unsigned char> img(
        WIDTH * HEIGHT * 3,
        255
    );

    //--------------------------------------------------
    // BOUNDS
    //--------------------------------------------------

    double minX = 1e18;
    double minY = 1e18;
    double maxX = -1e18;
    double maxY = -1e18;

    for(const auto& nodo :
        instancia.nodos)
    {
        minX = std::min(minX, nodo.x);
        minY = std::min(minY, nodo.y);
        maxX = std::max(maxX, nodo.x);
        maxY = std::max(maxY, nodo.y);
    }

    if(maxX == minX) maxX += 1.0;
    if(maxY == minY) maxY += 1.0;

    // ---- MEJORA: MARGEN DE SEGURIDAD VIRTUAL (PADDING) ----
    // Evita que los nodos situados exactamente en el extremo queden cortados por la mitad
    double rangoX = maxX - minX;
    double rangoY = maxY - minY;
    minX -= rangoX * 0.04;
    maxX += rangoX * 0.04;
    minY -= rangoY * 0.04;
    maxY += rangoY * 0.04;

    //--------------------------------------------------
    // MARGENES DEL LIENZO
    //--------------------------------------------------

    constexpr int LEFT   = 120;
    constexpr int RIGHT  = 80;
    constexpr int TOP    = 80;
    constexpr int BOTTOM = 120;

    auto normX =
        [&](double x)
    {
        return
            LEFT +
            (int)(
                (x - minX)
                /
                (maxX - minX)
                *
                (WIDTH - LEFT - RIGHT)
            );
    };

    auto normY =
        [&](double y)
    {
        return
            HEIGHT -
            (
                BOTTOM +
                (int)(
                    (y - minY)
                    /
                    (maxY - minY)
                    *
                    (HEIGHT - TOP - BOTTOM)
                )
            );
    };

    Color negro = { 0, 0, 0 };

    //--------------------------------------------------
    // EJES
    //--------------------------------------------------

    drawLine(img, LEFT, HEIGHT - BOTTOM, WIDTH - RIGHT, HEIGHT - BOTTOM, negro);
    drawLine(img, LEFT, HEIGHT - BOTTOM, LEFT, TOP, negro);

    drawText(img, WIDTH - 50, HEIGHT - BOTTOM + 20, "X", negro);
    drawText(img, 30, TOP - 20, "Y", negro);

    //--------------------------------------------------
    // GRADUACION X
    //--------------------------------------------------

    const int divisiones = 10;

    for(int i = 0; i <= divisiones; i++)
    {
        int x = LEFT + i * (WIDTH - LEFT - RIGHT) / divisiones;
        drawLine(img, x, HEIGHT - BOTTOM - 5, x, HEIGHT - BOTTOM + 5, negro);

        int valor = static_cast<int>(minX + i * (maxX - minX) / divisiones);
        drawText(img, x - 10, HEIGHT - BOTTOM + 20, std::to_string(valor), negro);
    }

    //--------------------------------------------------
    // GRADUACION Y
    //--------------------------------------------------

    for(int i = 0; i <= divisiones; i++)
    {
        int y = HEIGHT - (BOTTOM + i * (HEIGHT - TOP - BOTTOM) / divisiones);
        drawLine(img, LEFT - 5, y, LEFT + 5, y, negro);

        int valor = static_cast<int>(minY + i * (maxY - minY) / divisiones);
        drawText(img, 10, y - 5, std::to_string(valor), negro);
    }

    //--------------------------------------------------
    // RUTAS (Mejorado con colores por depósito y tonos)
    //--------------------------------------------------

    // Mapa para contar cuántas rutas hemos procesado por cada depósito (para los tonos)
    std::unordered_map<int, int> contadorRutasPorDeposito;

    for(const auto& ruta : solucion.rutas)
    {
        // Encontrar el índice de orden real del depósito en la instancia
        int depIdx = instancia.depositoToIndex.at(ruta.depositoId);
        
        // Seleccionamos el color base correspondiente al depósito
        Color colorBase = COLORES_DEPOSITOS[depIdx % NUM_COLORES_BASE];

        // Calculamos qué variación o tono le toca a esta ruta específica
        int indiceTono = contadorRutasPorDeposito[ruta.depositoId]++;
        Color colorRuta = generarTonoRuta(colorBase, indiceTono);

        const Nodo& deposito = instancia.nodos[depIdx];

        int prevX = normX(deposito.x);
        int prevY = normY(deposito.y);

        for(int clienteId : ruta.clientes)
        {
            int idx = instancia.clienteToIndex.at(clienteId);
            const Nodo& cliente = instancia.nodos[idx];

            int x = normX(cliente.x);
            int y = normY(cliente.y);

            drawLine(img, prevX, prevY, x, y, colorRuta);

            prevX = x;
            prevY = y;
        }

        // Conexión de regreso al depósito
        drawLine(img, prevX, prevY, normX(deposito.x), normY(deposito.y), colorRuta);
    }

    //--------------------------------------------------
    // NODOS (Siempre se dibujan al final por encima de las líneas)
    //--------------------------------------------------

    for(const auto& nodo : instancia.nodos)
    {
        int x = normX(nodo.x);
        int y = normY(nodo.y);

        if(nodo.esDeposito)
        {
            int depIdx = instancia.depositoToIndex.at(nodo.id);
            Color colorBase = COLORES_DEPOSITOS[depIdx % NUM_COLORES_BASE];

            // Dibujamos el depósito con su color base identificativo único
            drawSquare(img, x, y, 10, colorBase);
            drawText(img, x + 15, y - 10, "D" + std::to_string(nodo.id), negro);
        }
        else
        {
            drawCircle(img, x, y, 4, negro);
        }
    }

    //--------------------------------------------------
    // LEYENDA
    //--------------------------------------------------

    drawText(img, WIDTH - 300, 60, "LEYENDA", negro);
    drawSquare(img, WIDTH - 280, 110, 10, {128, 128, 128}); // Gris genérico para la muestra
    drawText(img, WIDTH - 240, 100, "DEPOSITO", negro);

    drawCircle(img, WIDTH - 280, 160, 5, negro);
    drawText(img, WIDTH - 240, 150, "CLIENTE", negro);

    drawLine(img, WIDTH - 290, 210, WIDTH - 240, 210, {128, 128, 128});
    drawText(img, WIDTH - 220, 200, "RUTA", negro);

    //--------------------------------------------------
    // GUARDAR ARCHIVO
    //--------------------------------------------------

    std::ofstream out(archivo, std::ios::binary);
    out << "P6\n" << WIDTH << " " << HEIGHT << "\n255\n";

    out.write(
        reinterpret_cast<char*>(img.data()),
        img.size()
    );

    out.close();
}