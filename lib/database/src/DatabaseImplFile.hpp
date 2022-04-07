#pragma once


#include <SPIFFS.h>


#include <types.hpp>


#define FILE_CSV_SEPARATOR ','
#define FILE_TMP_EXTENSION "tmp"


namespace DC {


    /**
     * @brief Manages a file that stores NFC-Tokens in the .csv format.
     * 
     * @author David Gruen
     */
    class DatabaseImplFile {

    public:

        /**
         * @brief Construct a new Database Impl File object
         * 
         * @param filename The name of the file including path.
         */
        DatabaseImplFile(const Filename& filename);


        /**
         * @brief Insert a NFC-Token into the file.
         * 
         * @param token 
         */
        void insert(const NFCToken& token);


        /**
         * @brief Remove a NFC-Token from the file.
         * 
         * @param token 
         */
        void remove(const NFCToken& token);


        /**
         * @brief Checks if a NFC-Token is in the database.
         * 
         * @param token 
         * @return true If token in database.
         * @return false Else.
         */
        bool contains(const NFCToken& token) const;


        /**
         * @brief Clears the file.
         * 
         */
        void clear();


        /**
         * @brief Applies function to every single NFC-Token.
         * 
         * @tparam Function Signature 'bool f(const NFCToken&)'
         *         where foreach stops if f returns true.
         * @param function 
         */
        template<typename Function>
        void foreach(Function function) const;

    private:

        void beginSPIFFS() const;

        bool existsFile() const;

        void createFile() const;

        void removeFile() const;

    private:

        const Filename filename;
        bool spiffsBegun;

    };


    template<typename Function>
    void DatabaseImplFile::foreach(Function function) const {
        this->beginSPIFFS();
        if (!this->existsFile()) {
            return;
        }
        File f = SPIFFS.open(this->filename, FILE_READ);
        String tokenStr = f.readStringUntil(FILE_CSV_SEPARATOR);
        while(!tokenStr.isEmpty()) {
            if (function(NFCToken(tokenStr))) {
                break;
            }
            tokenStr = f.readStringUntil(FILE_CSV_SEPARATOR);
        }
        f.close();
    }



};