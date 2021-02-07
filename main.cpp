# include "qrcode.h"

# include <cstdio>

int main(int argc, char *argv[]) {
    string str = "abcdefghijklmnopqrstuvwxyzabcdefghijkl";
    QRCode *qrcode = new QRCode(str, QRCode::QUITE);
    vector<vector<QRCode::Color>> pattern = qrcode->getPattern();
    for (int i=0; i<pattern.size(); ++i) {
        for (int j=0; j<pattern[i].size(); ++j) {
            if (pattern[i][j]==QRCode::DARK) printf("++");
            else printf("--");
        }
    }
    printf("\n");
    delete qrcode;
    return 0;
}