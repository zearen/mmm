#include <iostream>

#include "Region.h"

using namespace std;

void twrite (Region& br, int x, int z, int len, char c);
void tread  (Region& br, int x, int z);

int main(int argc, char **argv) {
    int choice, x, z, len;
    char inchar;
    if (argc < 2) return 1;
    Region br(argv[1]);
    do {
        cout << ": ";
        cin >> choice;
        switch (choice) {
            case 1:
                cout << "Writing\nx: ";
                cin >> x;
                cout << "z: ";
                cin >> z;
                cout << "len ";
                cin >> len;
                cout << "char: ";
                cin >> inchar;
                twrite(br, x, z, len, inchar);
                break;
            case 2:
                cout << "Reading\nx: ";
                cin >> x;
                cout << "z: ";
                cin >> z;
                tread(br, x, z);
                break;
            case 3:
                cout << "Deleting\nx: ";
                cin >> x;
                cout << "z: ";
                cin >> z;
                br.erase(x, z);
                break;
            default:
                choice = 0;
        }
    } while (choice);
    cout << "Quiting\n";
}

void twrite (Region& br, int x, int z, int len, char c) {
    Region::chunk_t chunk(len, R_ZLIB);
    for (int i = 0; i < len; i++) {
        chunk.data[i] = c;
    }
    br.write(x, z, chunk);
}


void tread  (Region& br, int x, int z) {
    Region::chunk_t chunk;
    try {
        chunk = br.read(x, z);
        cout << chunk.len << " " << (short)chunk.cmpr
            << ":";
        cout.flush();
        cout << chunk.data[0] << endl;
        delete[] chunk.data;
    }
    catch (ChunkOutOfRangeError e) {
        cout << "Invalid chunk\n";
    }
    catch (ChunkDoesNotExistError e) {
        cout << "Chunk does not exist\n";
    }
}
