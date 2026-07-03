#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <TFile.h>

#include <ROOT/REntry.hxx>
#include <ROOT/RFieldToken.hxx>
#include <ROOT/RNTupleModel.hxx>
#include <ROOT/RNTupleReadOptions.hxx>
#include <ROOT/RNTupleReader.hxx>
#include <ROOT/RNTupleWriteOptions.hxx>
#include <ROOT/RNTupleWriter.hxx>
#include <ROOT/RRawPtrWriteEntry.hxx>
#include <ROOT/RVersion.hxx>

namespace Framework {

struct Model {
    template <typename K>
    void RegisterField(K *address, std::string_view name) {
        fRNT_Model->MakeField<K>(std::string(name));
        fTokens.push_back(fRNT_Model->GetToken(name));
        fDataAddresses.push_back(static_cast<void *>(address));
    }

    template <typename Entry>
    void BindAll(Entry *entry) {
        for (std::size_t i = 0; i < fTokens.size(); ++i) entry->BindRawPtr(fTokens[i], fDataAddresses[i]);
    }

    std::unique_ptr<ROOT::RNTupleModel> fRNT_Model{ROOT::RNTupleModel::CreateBare()};
    std::vector<ROOT::RFieldToken> fTokens;
    std::vector<void *> fDataAddresses;
};

struct Writer {

    Writer(Framework::Model model, std::string_view rnt_name, TFile &output_file,
           const ROOT::RNTupleWriteOptions &options = ROOT::RNTupleWriteOptions())  //
        : fModel{std::move(model)} {
        fRNT_Writer = ROOT::RNTupleWriter::Append(std::move(fModel.fRNT_Model), rnt_name, output_file, options);
        fEntry = fRNT_Writer->GetModel().CreateRawPtrWriteEntry();
        fModel.BindAll(fEntry.get());
    }

    void Fill() { fRNT_Writer->Fill(*fEntry); }

    Framework::Model fModel;
    std::unique_ptr<ROOT::RNTupleWriter> fRNT_Writer;
    std::unique_ptr<ROOT::Detail::RRawPtrWriteEntry> fEntry;
};

struct Reader {

    Reader(Framework::Model model, std::string_view rnt_name, TFile &input_file,
           const ROOT::RNTupleReadOptions &options = ROOT::RNTupleReadOptions())  //
        : fModel{std::move(model)} {
        fRNT_Reader = ROOT::RNTupleReader::Open(std::move(fModel.fRNT_Model), rnt_name, input_file.GetName(), options);
        fEntry = fRNT_Reader->GetModel().CreateBareEntry();
        fModel.BindAll(fEntry.get());
    }

    void Load(ROOT::NTupleSize_t entryId) { fRNT_Reader->LoadEntry(entryId, *fEntry); }
    ROOT::RNTupleReader *Iter() { return fRNT_Reader.get(); }

    Framework::Model fModel;
    std::unique_ptr<ROOT::RNTupleReader> fRNT_Reader;
    std::unique_ptr<ROOT::REntry> fEntry;
};

}  // namespace Framework
