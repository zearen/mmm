BetaRegion::chunk_t::chunk_t(short length, byte compression) {
    cmpr = compression;
    if (length > 0) {
        len = length;
        data = new byte[len];
    }
    else {
        len = 0;
    }
}


BetaRegion::chunk_t::~chunk_t() {
    if (len > 0) {
        delete[] data;
    }
}

BetaRegion::BetaRegion(char *filename) {
    regionFile.open(filename, ios::in | ios::out | ios::binary);
    if (!regionFile.is_open())
        throw FileNotFound();
    regionFile.seek(0, ios::end);
    freeSectors.resize((regionFile.tellg() >> 8) + 1, 0xFF);
    regionFile.seek(0);
    
    // Let's scan for free sectors
    
}

BetaRegion::~BetaRegion() {
    regionFile.close()
}
