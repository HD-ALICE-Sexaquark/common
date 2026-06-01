#pragma once

#include <memory>

#include <TFile.h>

#include <ROOT/REntry.hxx>
#include <ROOT/RFieldToken.hxx>
#include <ROOT/RNTupleModel.hxx>
#include <ROOT/RNTupleReader.hxx>
#include <ROOT/RNTupleWriter.hxx>
#include <ROOT/RRawPtrWriteEntry.hxx>

namespace Framework {

struct Model {
    Model() : fRNT_Model{ROOT::RNTupleModel::CreateBare()} {}

    template <typename K>
    void RegisterField(void *address, std::string_view name) {
        fRNT_Model->MakeField<K>(std::string(name));
        fTokens.push_back(fRNT_Model->GetToken(name));
        fDataAddresses.push_back(address);
    }

    template <typename Entry>
    void BindAll(Entry *entry) {
        for (std::size_t i = 0; i < fTokens.size(); ++i) entry->BindRawPtr(fTokens[i], fDataAddresses[i]);
    }

    std::unique_ptr<ROOT::RNTupleModel> fRNT_Model;
    std::vector<ROOT::RFieldToken> fTokens;
    std::vector<void *> fDataAddresses;
};

template <typename T>
struct Writer {

    Writer(std::string_view rnt_name, TFile &output_file)  //
        : fModel{fPOD.CreateModel()} {
        fRNT_Writer = ROOT::RNTupleWriter::Append(std::move(fModel.fRNT_Model), rnt_name, output_file);
        fEntry = fRNT_Writer->GetModel().CreateRawPtrWriteEntry();
        fModel.BindAll(fEntry.get());
    }

    void Fill() { fRNT_Writer->Fill(*fEntry); }

    T &Data() { return fPOD; }
    const T &Data() const { return fPOD; }

    T fPOD{};
    Framework::Model fModel;
    std::unique_ptr<ROOT::RNTupleWriter> fRNT_Writer;
    std::unique_ptr<ROOT::Detail::RRawPtrWriteEntry> fEntry;
};

template <typename T>
struct Reader {

    Reader(std::string_view rnt_name, TFile &input_file)  //
        : fModel{fPOD.CreateModel()} {
        fRNT_Reader = ROOT::RNTupleReader::Open(std::move(fModel.fRNT_Model), rnt_name, input_file.GetName());
        fEntry = fRNT_Reader->GetModel().CreateBareEntry();
        fModel.BindAll(fEntry.get());
    }

    void Load(ROOT::NTupleSize_t entryId) { fRNT_Reader->LoadEntry(entryId, *fEntry); }
    ROOT::RNTupleReader *Iter() { return fRNT_Reader.get(); }

    T &Data() { return fPOD; }
    const T &Data() const { return fPOD; }

    T fPOD{};
    Framework::Model fModel;
    std::unique_ptr<ROOT::RNTupleReader> fRNT_Reader;
    std::unique_ptr<ROOT::REntry> fEntry;
};

}  // namespace Framework
