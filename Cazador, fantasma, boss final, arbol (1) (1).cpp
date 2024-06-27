int pocimas_salud = 1;//Este valor es universal para todas las pocimas de salud 

class Cazador {
private:
    int salud;
    int daño;
    int monedas;
    bool con_mision;

public:
    Cazador(int saludInicial, int dañoInicial, int monedasIniciales)
        : salud(saludInicial), daño(dañoInicial), monedas(monedasIniciales) {}

    void hacerDaño(Fantasma &fantasma);
    void hacerDaño(BossFinal &boss);
    void perderSalud(int cantidad);
    void ganarMonedas(int cantidad);
    void perderMonedas(int cantidad);
    void recuperarSalud(int cantidad);
    void moverse();
    int obtenerSalud() const { return salud; }
    int obtenerMonedas() const { return monedas; }
    void set_mision();
};

//Sirve para utilizar composicion en la clase persona para que estos puedan asignar misiones al cazador
class Mision{
    public:
    bool completado;
    Mision(){completado = false;}
};

class Persona{
    protected:
    int salud;
    Mision mission;

    public:
    void perder_salud(int);
    void ganar_salud(int);
    void dar_mision(Cazador &personaje){//Para asignar una mision a nuestro personaje
        personaje.set_mision();
    }

};


class Vendedor: public Persona{
    private:
    int daño;
    int monedas;
    int pocimas;//Recuerden que pocimas es un valor ya definido en el inico del codigo

    public:
    void ganar_monedas(int);
    void perder_monedas(int);
    void hacer_daño(Cazador &);
    void recibir_daño(int);

};


class Fantasma {
private:
    int vida;
    int daño;
    int recompensa;

public:
    Fantasma(int vidaInicial, int dañoInicial, int recompensaInicial)
        : vida(vidaInicial), daño(dañoInicial), recompensa(recompensaInicial) {}

    void recibirDaño(int cantidad) {
        vida -= cantidad;
        if (vida < 0) vida = 0;
    }

    void hacerDaño(Cazador &cazador) {
        cazador.perderSalud(daño);
    }

    int soltarMonedas() const { return recompensa; }
    int obtenerVida() const { return vida; }
    int obtenerDaño() const { return daño; }

};



class BossFinal {
private:
    int vida;
    int daño;
    int monedas;

public:
    BossFinal(int vidaInicial, int dañoInicial, int monedasIniciales)
        : vida(vidaInicial), daño(dañoInicial), monedas(monedasIniciales) {}

    void recibirDaño(int cantidad) {
        vida -= cantidad;
        if (vida < 0) vida = 0;
    }

    void hacerDaño(Cazador &cazador) {
        cazador.perderSalud(daño);
    }

    int soltarMonedas() const { return monedas; }
    int obtenerVida() const { return vida; }
    int obtenerDaño() const { return daño; }

};



class Arbol {
private:
    int salud;

public:
    Arbol(int saludInicial) : salud(saludInicial) {}

    void noDejarPasar();
    int obtenerSalud() const { return salud; }

};


//===========================================================================================//
//Metodos de cazador
void Cazador::hacerDaño(Fantasma &fantasma) {
    fantasma.recibirDaño(daño);
    if (fantasma.obtenerVida() == 0) {
        ganarMonedas(fantasma.soltarMonedas());
    }
}


void Cazador::hacerDaño(BossFinal &boss) {
    boss.recibirDaño(daño);
    if (boss.obtenerVida() == 0) {
        ganarMonedas(boss.soltarMonedas());
    }
}


void Cazador::perderSalud(int cantidad) {
    salud -= cantidad;
    if (salud < 0) salud = 0;
}


void Cazador::ganarMonedas(int cantidad) {
    monedas += cantidad;
}


void Cazador::perderMonedas(int cantidad) {
    monedas -= cantidad;
    if (monedas < 0) monedas = 0;
}


void Cazador::recuperarSalud(int cantidad) {
    salud += cantidad;
}
//Falta definir el metodo cazador caminar 

void Cazador::set_mision(){
    con_mision = true;
}

//==========================================================================================//
//Metodos de las personas y de los vendedores
void Vendedor::ganar_monedas(int coins){
    monedas += coins;
}

void Vendedor::recibir_daño(int damage){
    salud -= damage;
}

void Vendedor::hacer_daño(Cazador &target){
    target.perderSalud(daño);
}

void Vendedor::perder_monedas(int coins){
    monedas -= coins;
}


void Vendedor::ganar_monedas(int coins){
    monedas += coins;
}

void Persona::perder_salud(int danio){
    salud -= danio;
}

void Persona::ganar_salud(int health){
    salud += health;
}
