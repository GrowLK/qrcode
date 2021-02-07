# ifndef QRCODE_H
# define QRCODE_H

# include <vector>
using std::vector;
# include <string>
using std::string;
# include <map>
using std::map;

class QRCode {
    public:
        /* Two kinds of color of module. */
        enum Color { LIGHT, DARK };
        /* Four kinds of encoding modes. */
        enum Mode { NUMERIC, ALPHANUMERIC, BYTE, KANJI, UNKNOWNMODE };
        /* Four kinds of error correction level providing 7%, 15%, 25%, 30% error correction. */
        enum Level { LOW, MIDDLE, QUITE, HIGH, UNKNOWNLEVEL };
        /* A item including coefficient and exponent of polynomial. */
        struct Polynomial { unsigned char coefficient; unsigned char exponent; };
        /* Coordinate of module. */
        struct Coordinate { unsigned char xValue; unsigned char yValue; };
        /* Eight kinds of mask mode. */
        enum Mask { ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, UNKNOWNMASK };
        /* The coding sequence. */
        struct Sequence { vector<unsigned long> sequence; unsigned short length; };

    private:
        static vector<unsigned char> modeIndicator;
        static vector<vector<unsigned int>> charCapacity;
        static vector<vector<unsigned char>> countLength;
        static map<char, unsigned short> alphaValue;
        static vector<vector<vector<unsigned short>>> codewordNumber;
        static vector<unsigned char> logTable;
        static vector<unsigned char> antilogTable;
        static vector<unsigned char> remainderBit;
        static vector<vector<unsigned char>> centerLocation;
        static vector<vector<unsigned short>> formatString;
        static vector<unsigned int> versionString;

        /* The ECI designator. */
        unsigned char eci;
        /* Encoding mode used by the QR-code. */
        Mode mode;
        /* Error correction level used by the QR-code */
        Level level;
        /* Mask mode used by the QR-code. */
        Mask mask;
        /* Data stored in the QR-code. */
        string data;
        /* Version used by the QR-code. */
        unsigned char version;
        /* Size of the QR-code. */
        unsigned char size;
        /* sequence of data bits and error correction bits. */
        Sequence sequence;
        /* Data codeword groups and blocks. */
        vector<vector<unsigned char>> dataBlocks;
        /* Error correction codeword groups and blocks. */
        vector<vector<unsigned char>> ecBlocks;
        /* Pattern of the QR-code */
        vector<vector<Color>> pattern;

        void encode(Sequence &seq, unsigned long val, unsigned char len);
        unsigned long decode(const Sequence &seq, unsigned short pos, unsigned char len);
        vector<Polynomial> multiply(const vector<Polynomial> &A, const vector<Polynomial> &B);
        vector<Polynomial> divide(const vector<Polynomial> &A, const vector<Polynomial> &B);
        unsigned int penalize(vector<vector<Color>> pat);

        void numericEncoding(Sequence &seq, const string &data);
        void alphanumericEncoding(Sequence &seq, const string &data);
        void byteEncoding(Sequence &seq, const string &data);
        void kanjiEncoding(Sequence &seq, const string &data);
        unsigned char selectEci(const string &data);
        Mode selectMode(const string &data, unsigned char eci);
        unsigned char selectVersion(const string &data, Mode mode, Level level);
        void encodeData(Sequence &seq, const string &data);
        void decodeData(const Sequence &seq, string &data);
        void drawPattern(vector<vector<Color>> &pat, const Sequence &seq);
        void scanPattern(const vector<vector<Color>> &pat, Sequence &seq);

    public:
        QRCode();
        QRCode(const string &data, const Level &level);
        QRCode(QRCode &qrcode);
        ~QRCode();
        void createQrcode(const string &data, const Level &level);
        unsigned char getEci();
        unsigned char getVersion();
        unsigned char getSize();
        char* getMode();
        char* getLevel();
        char* getData();
        vector<vector<Color>> getPattern();
};

# endif /* QRCODE_H */