#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

struct Resultado
{
    double fitness = 0.0;
    double penalizacion = 0.0;
    int vehiculos = 0;
};

Resultado leerResultado(
    const std::string& archivo
)
{
    Resultado r;

    std::ifstream in(archivo);

    std::string palabra;

    while(in >> palabra)
    {
        if(palabra == "Fitness:")
        {
            in >> r.fitness;
        }
        else if(palabra == "Penalizacion:")
        {
            in >> r.penalizacion;
        }
        else if(palabra == "Vehiculos:")
        {
            in >> r.vehiculos;
        }
    }

    return r;
}

int main()
{
    namespace fs =
        std::filesystem;

    std::ofstream csv(
        "output/resultados_recuperados.csv"
    );

    csv
    << "Instancia,"
    << "FitnessGreedy,"
    << "FitnessTabu,"
    << "Mejora,"
    << "PenalizacionGreedy,"
    << "PenalizacionTabu,"
    << "VehiculosGreedy,"
    << "VehiculosTabu\n";

    std::map<
        std::string,
        Resultado
    > greedy;

    std::map<
        std::string,
        Resultado
    > tabu;

    for(const auto& carpeta :
        fs::directory_iterator(
            "output"
        ))
    {
        if(!carpeta.is_directory())
        {
            continue;
        }

        for(const auto& archivo :
            fs::directory_iterator(
                carpeta.path()
            ))
        {
            std::string nombre =
                archivo.path()
                .filename()
                .string();

            std::string ruta =
                archivo.path()
                .string();

            if(
                nombre.find(
                    "_greedy.txt"
                ) != std::string::npos
            )
            {
                std::string instancia =
                    nombre.substr(
                        0,
                        nombre.find(
                            "_greedy.txt"
                        )
                    );

                greedy[instancia] =
                    leerResultado(
                        ruta
                    );
            }

            if(
                nombre.find(
                    "_tabu.txt"
                ) != std::string::npos
            )
            {
                std::string instancia =
                    nombre.substr(
                        0,
                        nombre.find(
                            "_tabu.txt"
                        )
                    );

                tabu[instancia] =
                    leerResultado(
                        ruta
                    );
            }
        }
    }

    for(const auto& [instancia, g]
        : greedy)
    {
        if(
            tabu.find(instancia)
            == tabu.end()
        )
        {
            continue;
        }

        Resultado t =
            tabu[instancia];

        csv
        << instancia << ","
        << g.fitness << ","
        << t.fitness << ","
        << (g.fitness - t.fitness)
        << ","
        << g.penalizacion << ","
        << t.penalizacion << ","
        << g.vehiculos << ","
        << t.vehiculos
        << "\n";
    }

    csv.close();

    std::cout
        << "CSV reconstruido: "
        << "output/resultados_recuperados.csv\n";

    return 0;
}