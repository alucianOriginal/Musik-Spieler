// Philosophie:
// Festgelegte Grenzen
// Maximale Kompatibilitaet und Qualitaet!
// OPEN SOURCE/ FREE FOR ALL AND EVERYBODY :-)
// Name: Musik Spieler
// Version: 0.3 WAV Only Version
// Funktion Bedingungen
// Start: Leertaste, oder Starttaste Geraet
// Stopp: Leertaste, oder Stopptaste Gereat
// Weiter: Pfeil Rechts
// Zurueck: Pfeil Links
// Laut Pfeil Hoch, oder Lautertaste Geraet
// Leise Pfeil Runter, oder Leisertaste Geraet
// Absolut abstrahiert arbeiten!

#include <fstream>
#include <iostream>
#include <cstdint>

#define MIN_SAMPLERATE 44100   // Minimum Gute Sound Diktatur
#define MAX_SAMPLERATE 192000  // Maximum High-End
#define BUFFER_SIZE 4096       // Statischer Puffer Klein Cache, groß Knacken
#define MAX_PLAYLIST 256       // Begrenzung Liste RAM-Explosion
#define MIN_PLAYLIST 0         //
#define MIN_VOLUME 0           // Mindestlautstaerke
#define MAX_VOLUME 100         // Schutz Übersteuerung Knarzen
#define LIST_FILENAME          "MusikSpielerListe.txt"

//StrukturWAV-KopfZEILEN)
struct WAVHeader {
    char riff[4];           //MussRIFFsein
    uint32_t fileSize;
    char wave[4];           //MussWAVEsein
    char fmt[4];            //fmt
    uint32_t fmtLen;
    uint16_t formatTag;     //1=PCM
    uint16_t channels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
    char data[4];
    uint32_t dataLen;
};

class MusikSpieler {
private:
    bool isPlaying = false;
    float volumeMultiplier = 0.25f;
    float targetVolume = 0.15f;
    float audioBuffer[BUFFER_SIZE];
    const char* playlist[MAX_PLAYLIST] = {nullptr};
    int volume = 25;
    int currentTrack = 0;
    unsigned int sampleRate = 44100;
    bool loadWAV(const char* filepath) {
        std::ifstream file(filepath, std::ios::binary);
        if (!file) return false;
        WAVHeader header;
        file.read(reinterpret_cast<char*>(&header), sizeof(WAVHeader));
        if (std::string(header.riff, 4) != "RIFF" || std::string(header.wave, 4) != "WAVE") {
            return false;
        }
        this->sampleRate = header.sampleRate;
        return true;
    }
public:
    //Steuerung
    void key_Space()  { toggle(); }        //Start/Stopp
    void key_Right()  { next(); }          //Weiter
    void key_Left()   { back(); }          //Zurueck
    void key_Up()     { setVolume(volume + 5); } //Lauter
    void key_Down()   { setVolume(volume - 5); } //Leiser
    //SCHNITTSTELLE
    void setQuality(unsigned int rate) {
        if (rate >= MIN_SAMPLERATE && rate <= MAX_SAMPLERATE) {
            this->sampleRate = rate;
        }
    }
    //PLAYLIST
    void start() { isPlaying = true; }
    void toggle() { isPlaying = !isPlaying; }
    void next() {
        if (currentTrack < MAX_PLAYLIST - 1 && playlist[currentTrack + 1])
            currentTrack++;
    }
    void back() {
        if (currentTrack > 0) currentTrack--;
    }
    void setVolume(int v) {
        if (v < 0) v = 0;
        if (v > MAX_VOLUME) v = MAX_VOLUME;
        this->volume = v;
        targetVolume = (float)v / 100.0f;
    }
    void addTrack(const char* path) {
        for(int i = 0; i < MAX_PLAYLIST; i++) {
            if(playlist[i] == nullptr) {
                playlist[i] = path;
                break;}
                playlist[MAX_PLAYLIST - 1] = nullptr;
                    if (currentTrack >= index && currentTrack > 0)
                    currentTrack--;
                    }
                }
            }
        }
    void removeTrack(int index) {
        if (index < 0 || index >= MAX_PLAYLIST) return;
        if (playlist[index] == nullptr) return;

        for (int i = index; i < MAX_PLAYLIST - 1; i++) {
        playlist[i] = playlist[i + 1];
        if (playlist[i] == nullptr) break;
                }
                playlist[MAX_PLAYLIST - 1] = nullptr;
                if (currentTrack >= index && currentTrack > 0)
                    currentTrack--;
                }
            }
        }
    void savePlaylist() {
    std::ofstream file(LIST_FILENAME);
    for(int i = 0; i < MAX_PLAYLIST && playlist[i]; i++)
        file << playlist[i] << "\n";
            }
            //TREIBERVERWALTUNG
            void processAudio(float* deviceOutput, float* decoderInput, int length) {
            if (!isPlaying) {
            for(int i = 0; i < length; i++) deviceOutput[i] = 0.0f;
            return;
            }
            for(int i = 0; i < length; i++) {
                //AntiKnackRampe
                volumeMultiplier += (targetVolume - volumeMultiplier) * 0.001f;
            float sample = decoderInput[i] * volumeMultiplier;
            if (sample > 1.0f)  sample = 1.0f;
            if (sample < -1.0f) sample = -1.0f;
            deviceOutput[i] = sample;
        }
    }
};
