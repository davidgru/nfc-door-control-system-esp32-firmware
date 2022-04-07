#include "DatabaseImplFile.hpp"


namespace DC {

    
    DatabaseImplFile::DatabaseImplFile(const Filename& filename)
        : filename(filename) {

    }


    void DatabaseImplFile::insert(const NFCToken& token) {
        this->beginSPIFFS();
        if (!this->existsFile()) {
            this->createFile();
        }
        File f = SPIFFS.open(this->filename, FILE_APPEND);
        f.print(token.toHexRepr());
        f.print(FILE_CSV_SEPARATOR);
        f.close();
    }


    void DatabaseImplFile::remove(const NFCToken& token) {
        this->beginSPIFFS();
        const Filename tmpName = this->filename + FILE_TMP_EXTENSION;
        File tmp = SPIFFS.open(tmpName, FILE_WRITE);
        this->foreach([&](const NFCToken& readToken) -> bool {
            if (token != readToken) {
                tmp.print(readToken.toHexRepr());
                tmp.print(FILE_CSV_SEPARATOR);
            }
            return false;
        });
        tmp.close();
        this->removeFile();
        SPIFFS.rename(tmpName, filename);
    }


    bool DatabaseImplFile::contains(const NFCToken& token) const {
        this->beginSPIFFS();
        bool found = false;
        this->foreach([&](const NFCToken& readToken) -> bool {
            if (token == readToken) {
                found = true;
                return true;
            }
            return false;
        });
        return found;
    }


    void DatabaseImplFile::clear() {
        this->beginSPIFFS();
        if (!this->existsFile()) {
            return;
        }
        this->removeFile();
    }


    void DatabaseImplFile::beginSPIFFS() const {
        if (!this->spiffsBegun) {
            SPIFFS.begin();
        }
    }


    bool DatabaseImplFile::existsFile() const {
        this->beginSPIFFS();
        return SPIFFS.exists(this->filename);
    }


    void DatabaseImplFile::createFile() const {
        this->beginSPIFFS();
        File f = SPIFFS.open(this->filename, FILE_WRITE);
        f.close();
    }


    void DatabaseImplFile::removeFile() const {
        this->beginSPIFFS();
        SPIFFS.remove(this->filename);
    }


}
