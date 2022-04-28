#ifndef INPUT_HANDLER_H__
#define INPUT_HANDLER_H__

#include <fstream>
#include <iostream>
#include <string>

namespace {

    void buildPreprocessorBuffer (std::vector<std::string>& buf,
                                  std::ifstream& preCode)
    {
        std::string line;
        while (std::getline (preCode, line)) 
            buf.push_back (line);
    }

    void handleImportStmts (std::vector<std::string>& buffer) {

        const std::size_t skipSymbs = 7;

        std::size_t bufSize = buffer.size ();
        for (std::size_t i = 0; i < bufSize; ++i) {

            std::size_t importFound = buffer [i].find ("import");
            if (importFound != std::string::npos) {

                std::size_t importFilePos = importFound + skipSymbs;
                std::string fileToImport  = buffer [i].substr (importFilePos);
                
                std::ifstream importFile;
                importFile.open (fileToImport, std::ios::in);
                if (!importFile.is_open ()) { throw std::runtime_error ("Can't open import file"); }
                
                std::vector<std::string> importFileBuf;
                buildPreprocessorBuffer (importFileBuf, importFile);

                auto posToInsert = buffer.begin ();
                std::advance (posToInsert, i);

                buffer.insert (posToInsert, importFileBuf.begin (), importFileBuf.end ());
                
                auto posToErase = buffer.begin ();
                std::advance (posToErase, i + importFileBuf.size ());
                buffer.erase (posToErase);

                bufSize = buffer.size ();
                i = 0;

            }

        }

    }

}

struct Preprocessor final {

    std::vector<std::string> originalBuffer_;
    std::vector<std::string> preprocessedBuffer_;

    std::string preprocessedFileName_;

    Preprocessor (const char* inputFile) {

        std::ifstream preCode;
        preCode.open (inputFile, std::ios::in);
        if (!preCode.is_open ()) { throw std::runtime_error ("Can't open input file"); }

        buildPreprocessorBuffer (originalBuffer_, preCode);

        preprocessedBuffer_ = originalBuffer_;

        handleImportStmts (preprocessedBuffer_);

        std::string containStr;
        for (std::size_t i = 0; i < preprocessedBuffer_.size (); ++i) {
            
            containStr += preprocessedBuffer_ [i] + '\n';
        }

        preprocessedFileName_ = std::string (inputFile) + ".preprocessed";

        std::ofstream preprocessedFile (preprocessedFileName_);
        preprocessedFile << containStr;

    }

};

struct InputHandler final {
    int argc_ = 0;
    char **argv_ = nullptr;

    std::fstream code_;

    std::string msg_;

    InputHandler () {}

    void setParams (const int argc, char **argv)
    {
        argv_ = argv;
        argc_ = argc;

        if (argc_ != 2) {
            throw std::runtime_error ("You have to enter input filename only!");
        }

        Preprocessor preprocessor (argv_ [1]);
        code_.open (preprocessor.preprocessedFileName_);
        if (!code_.is_open ())
            throw std::runtime_error ("Can't open preprocessor file");

    }
};

#endif
