#ifndef WIIMOTE_H
#define WIIMOTE_H

#include <iostream>
#include <string>
#include <memory>
#include <vector>
//#include "/home/twsh/desarrollo/libwiimote-ng-v0.5.1/src/wiimote.h"
//#include "/home/twsh/desarrollo/libwiimote-ng-v0.5.1/src/wiimote_api.h"
#include <wiimote.h>
#include <wiimote_api.h>


using namespace std;

class WiimoteEvent
{
    public:
        WiimoteEvent(const wiimote_t &wiimote) : _wiimote(wiimote) {}
        virtual ~WiimoteEvent(){}
        const wiimote_t &_wiimote;
    private:
        // Prevención de copia (como en la libreria OIS)
        WiimoteEvent(const WiimoteEvent &);
        WiimoteEvent& operator=(WiimoteEvent);

};

class WiimoteListener
{
    public:
        virtual bool WiimoteButtonUp(const WiimoteEvent & arg) = 0;
        virtual bool WiimoteButtonDown(const WiimoteEvent & arg) = 0;
        virtual bool WiimoteIRMove(const WiimoteEvent & arg) = 0;
};

class Wiimote
{
    public:
        Wiimote();
        virtual ~Wiimote();

        size_t Connect(string hci_address); // Necesitará al miembro _wiimote y su dirección bluetooth para conectarse, algo como 00:15:83:15:A3:10.
        void ActivaLed(size_t led,bool activa);  // Activa el led indicado en el wiimote.
        size_t Update();                    // Para sincronizar el objeto wiimote con el wiimote real. Se debería llamar tantas veces como sea posible para minimizar la latencia.
        void Disconnect();                  // Desconecta el wiimote. Lo apaga.
        size_t Report();                    // Recupera un informe del estado actual del wiimote.
        void ActivaRumble(bool activar);    // Activa o desactiva el rumble del wiimote.
        void ActivaIR(bool activar);          // Activa o desactiva el sensor de infrarrojos.
        void ActivaAccelerator(bool activar); // Activa o desactiva el acelerómetro.
        bool Wiimote_is_open();
        wiimote_t getWiimoteStatus();
        bool AddWiimoteListener(unique_ptr<WiimoteListener> listener);
//      bool Home();
//      bool A();
//      bool B();
//      bool Plus();
//      bool Minus();
//      bool One();
//      bool Two();

    protected:

    private:
        unique_ptr<wiimote_t> _wiimote {new wiimote_t};
	    unique_ptr<wiimote_report_t> _report {new wiimote_report_t};
	    string _hci_address;
	    vector< unique_ptr<WiimoteListener> > _WiimoteListeners;
	    vector<wiimote_ir_t> _ir; // sirve para ir almacenando los valores IR y compararlos para ver si han cambiado y comunicárselo a un listener.
	    inline void setIRActual(); // Aquí es donde actualizamos los valores IR actuales
	    bool NuevosValoresIR();
	    wiimote_keys_t _oldBits; // Estado de los botones anterior al momento en el que se dispara un evento ButtonUP o ButtonDOWN
	    inline void setOldKeysBits();
	    bool NuevosKeysBits();
//   	bool _home;
//	    bool _A;
//	    bool _B;
//	    bool _Plus;
//	    bool _Minus;
//	    bool _One;
//	    bool _Two;

};

#endif // WIIMOTE_H
