# include "qrcode.h"

# include <cstdio>

int main(int argc, char *argv[]) {
    if (argc != 2 ) {
        printf("usage: ./main <message>");
        return 0;
    }
    string str(argv[1]);
    QRCode *qrcode = new QRCode(str, QRCode::QUITE);
    vector<vector<QRCode::Color>> pattern = qrcode->getPattern();
    for (int i=0; i<pattern.size(); ++i) {
        for (int j=0; j<pattern[i].size(); ++j) {
            if (pattern[i][j]==QRCode::DARK) printf("\033[44m  \033[0m");
            else printf("\033[47m  \033[0m");
        }
        printf("\n");
    }
    delete qrcode;
    return 0;
}
