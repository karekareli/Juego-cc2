#include <SFML/Graphics.hpp>
#include <stack>
#include <iostream>
#include <random>

#define TAMANIO 20
#define ANCHO_CELDA 20



class Celda {
public:
    int x, y;
    int pos;
    float tamano = 30.f;
    float grosor = 2.f;

    bool paredes[4] = { true,true,true,true };
    bool visitada = false;
    bool esActiva = false;

    Celda();
    Celda(int, int);
    void dibujar(sf::RenderWindow* ventana);
};

Celda::Celda() {}
Celda::Celda(int _x, int _y) {
    x = _x;
    y = _y;
}



void Celda::dibujar(sf::RenderWindow* ventana) {
    sf::RectangleShape rect;

    if (esActiva) {

        rect.setFillColor(sf::Color(165, 42, 42));
        rect.setSize(sf::Vector2f(tamano, tamano));

        rect.setPosition(x, y);
        ventana->draw(rect);
    }
    rect.setFillColor(sf::Color(223, 243, 228));


    if (paredes[0]) {
        rect.setSize(sf::Vector2f(tamano, grosor));

        rect.setPosition(x, y);
        ventana->draw(rect);
    }
 
    if (paredes[1]) {
        rect.setSize(sf::Vector2f(grosor, tamano));

        rect.setPosition(x + tamano, y);
        ventana->draw(rect);
    }
   
    if (paredes[2]) {
        rect.setSize(sf::Vector2f(tamano + grosor, grosor));
        rect.setPosition(x, y + tamano);
        ventana->draw(rect);
    }
  
    if (paredes[3]) {
        rect.setSize(sf::Vector2f(grosor, tamano));
        rect.setPosition(x, y);

        ventana->draw(rect);
    }
}

void reiniciarLaberinto(Celda* laberinto, int tamano) {

    for (int i = 0; i < tamano * tamano; i++) {
        for (int j = 0; j < 4; j++) {

            laberinto[i].paredes[j] = true;

            laberinto[i].visitada = false;
            laberinto[i].esActiva = false;
        }
    }
}



void eliminarParedesEntre(Celda* laberinto, Celda* actual, Celda* elegida, int tamano) {

    if (actual->pos - tamano == elegida->pos) {

        actual->paredes[0] = false;
        elegida->paredes[2] = false;
    }
 
    else if (actual->pos + 1 == elegida->pos) {
        actual->paredes[1] = false;
        elegida->paredes[3] = false;
    }
 
    else if (actual->pos + tamano == elegida->pos) {

        actual->paredes[2] = false;
        elegida->paredes[0] = false;
    }

    else if (actual->pos - 1 == elegida->pos) {
        actual->paredes[3] = false;
        elegida->paredes[1] = false;
    }
}


void crearLaberinto(Celda* laberinto, int tamano) {
    reiniciarLaberinto(laberinto, tamano);
    std::stack<Celda> pila;
    laberinto[0].visitada = true;
    pila.push(laberinto[0]);

    while (!pila.empty()) {
        Celda actual = pila.top();
        pila.pop();
        int pos = actual.pos;
        std::vector<int> vecinos;

        if ((pos) % (tamano) != 0 && pos > 0) {
            Celda izquierda = laberinto[pos - 1];

            if (!izquierda.visitada) {
                vecinos.push_back(pos - 1);
            }
        }

        if ((pos + 1) % (tamano) != 0 && pos < tamano * tamano) {
            Celda derecha = laberinto[pos + 1];
            if (!derecha.visitada) {
                vecinos.push_back(pos + 1);
            }
        }

        if ((pos + tamano) < tamano * tamano) {
            Celda abajo = laberinto[pos + tamano];
            if (!abajo.visitada) {
                vecinos.push_back(pos + tamano);
            }
        }

        if ((pos - tamano) > 0) {
            Celda arriba = laberinto[pos - tamano];
            if (!arriba.visitada) {
                vecinos.push_back(pos - tamano);
            }
        }

        if (vecinos.size() > 0) {
     
            std::random_device dev;
            std::mt19937 rng(dev());
            std::uniform_int_distribution<std::mt19937::result_type> dist6(0, vecinos.size() - 1);
            int posVecinoAleatorio = dist6(rng);

           
            Celda* elegida = &laberinto[vecinos[posVecinoAleatorio]];

            pila.push(actual);

            eliminarParedesEntre(laberinto, &laberinto[actual.pos], elegida, tamano);

            elegida->visitada = true;
            pila.push(*elegida);
        }
    }
}



void manejarMovimiento(sf::Event evento, Celda* laberinto, int* posActual, int tamano) {
    if (
        evento.key.code == sf::Keyboard::Left ||
        evento.key.code == sf::Keyboard::H
        ) {
      
        if (!laberinto[*posActual].paredes[3] && !laberinto[*posActual - 1].paredes[1]) {
            *posActual = *posActual - 1;
            laberinto[*posActual].esActiva = true;
        }
    }

    else if (
        evento.key.code == sf::Keyboard::Right ||
        evento.key.code == sf::Keyboard::L
        ) {
       
        if (!laberinto[*posActual].paredes[1] && !laberinto[*posActual + 1].paredes[3]) {
            *posActual = *posActual + 1;
            laberinto[*posActual].esActiva = true;
        }
    }

    else if (
        evento.key.code == sf::Keyboard::Up ||
        evento.key.code == sf::Keyboard::K
        ) {
        if ((*posActual - tamano) < 0) {
            return;
        }

        if (!laberinto[*posActual].paredes[0] && !laberinto[*posActual - tamano].paredes[2]) {
            *posActual = *posActual - tamano;
            laberinto[*posActual].esActiva = true;
        }
    }

    else if (
        evento.key.code == sf::Keyboard::Down ||
        evento.key.code == sf::Keyboard::J
        ) {
        if ((*posActual + tamano) > tamano * tamano) {
            return;
        }

        if (!laberinto[*posActual].paredes[2] && !laberinto[*posActual + tamano].paredes[0]) {
            *posActual = *posActual + tamano;
            laberinto[*posActual].esActiva = true;
        }
    }
}



class Cazador {
public:
    sf::RectangleShape rect;
    Cazador() {
        rect.setFillColor(sf::Color(0, 0, 0));
        rect.setSize(sf::Vector2f(ANCHO_CELDA, ANCHO_CELDA));
    }

    void setPosicion(int x, int y) {
        rect.setPosition(x, y);
    }

    void dibujar(sf::RenderWindow* ventana) {
        ventana->draw(rect);
    }
};



class Fantasma {
public:
    sf::RectangleShape rect;
    Fantasma() {
        rect.setFillColor(sf::Color(166, 207, 213));
        rect.setSize(sf::Vector2f(ANCHO_CELDA, ANCHO_CELDA));
    }

    void setPosicion(int x, int y) {
        rect.setPosition(x, y);
    }

    void dibujar(sf::RenderWindow* ventana) {
        ventana->draw(rect);
    }
};



int main(int argc, char* argv[]) {

    int posActual = 0;

    sf::RenderWindow ventana(sf::VideoMode(ANCHO_CELDA * TAMANIO + 60, ANCHO_CELDA * TAMANIO + 60), "Laberinto");
    Celda* laberinto = new Celda[TAMANIO * TAMANIO];
    ventana.setFramerateLimit(30);


    for (int i = 30, k = 0; i < ANCHO_CELDA * TAMANIO + 30; i += ANCHO_CELDA) {
        for (int j = 30; j < ANCHO_CELDA * TAMANIO + 30; j += ANCHO_CELDA, k++) {

            laberinto[k].y = i;
            laberinto[k].x = j;

            laberinto[k].tamano = ANCHO_CELDA;
            laberinto[k].pos = k;
        }
    }

    crearLaberinto(laberinto, TAMANIO);
    laberinto[posActual].esActiva = true;


    Cazador cazador;
    Fantasma fantasma;

    while (ventana.isOpen()) {
        sf::Event evento;
        while (ventana.pollEvent(evento)) {
            switch (evento.type) {

            case sf::Event::Closed:
                ventana.close();
                break;

            case sf::Event::KeyPressed:
                manejarMovimiento(evento, laberinto, &posActual, TAMANIO);
                break;

            default:
                break;
            }
        }

        if (posActual == (TAMANIO * TAMANIO - 1)) {
            crearLaberinto(laberinto, TAMANIO);
            posActual = 0;
            laberinto[posActual].esActiva = true;
        }

        ventana.clear(sf::Color(0, 100, 0));

        for (int i = 0; i < TAMANIO * TAMANIO; i++) {
            laberinto[i].dibujar(&ventana);
        }

        cazador.setPosicion(laberinto[posActual].x, laberinto[posActual].y);
        cazador.dibujar(&ventana);

        fantasma.setPosicion(laberinto[TAMANIO * TAMANIO - 1].x, laberinto[TAMANIO * TAMANIO - 1].y);
        fantasma.dibujar(&ventana);

        ventana.display();
    }

    delete[] laberinto;

    return 0;
}

