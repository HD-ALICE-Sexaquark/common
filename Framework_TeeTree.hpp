#pragma once

#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <TBranch.h>
#include <TFile.h>
#include <TTree.h>

namespace Framework::TeeTree {

// clang-format off
template <typename T>
constexpr const char* LeafCodeFor() {
    // doesn't accept long/unsigned long (whose width is platform-dependent)
    static_assert(std::is_arithmetic_v<T>, "LeafCodeFor: T must be arithmetic");
    // order matters: handle char/int8_t aliases explicitly.
    if      constexpr (std::is_same_v<T, bool>)               return "O";
    else if constexpr (std::is_same_v<T, char>)               return "B";  // signed by ROOT convention
    else if constexpr (std::is_same_v<T, signed char>)        return "B";
    else if constexpr (std::is_same_v<T, unsigned char>)      return "b";
    else if constexpr (std::is_same_v<T, std::int8_t>)        return "B";
    else if constexpr (std::is_same_v<T, std::uint8_t>)       return "b";
    else if constexpr (std::is_same_v<T, std::int16_t>)       return "S";
    else if constexpr (std::is_same_v<T, std::uint16_t>)      return "s";
    else if constexpr (std::is_same_v<T, std::int32_t>)       return "I";
    else if constexpr (std::is_same_v<T, std::uint32_t>)      return "i";
    else if constexpr (std::is_same_v<T, std::int64_t>)       return "L";
    else if constexpr (std::is_same_v<T, std::uint64_t>)      return "l";
    else if constexpr (std::is_same_v<T, float>)              return "F";
    else if constexpr (std::is_same_v<T, double>)             return "D";
    else static_assert(sizeof(T) == 0, "LeafCodeFor: unsupported arithmetic type");
}
// clang-format on

struct Model {

    struct Entry {
        std::string name;
        void* address{nullptr};
        std::shared_ptr<void> pptr;  // owns the T* used for class branches
        std::function<TBranch*(TTree&, const Entry&)> makeBranch;
        std::function<void(TTree&, const Entry&)> setAddress;
    };

    template <typename K>
    void RegisterBranch(K* address, std::string_view name) {
        Entry e;
        e.name = std::string(name);
        e.address = static_cast<void*>(address);
        if constexpr (std::is_arithmetic_v<K>) {
            e.makeBranch = [](TTree& t, const Entry& en) -> TBranch* {
                const std::string leaf = en.name + "/" + LeafCodeFor<K>();
                return t.Branch(en.name.c_str(), en.address, leaf.c_str());
            };
            e.setAddress = [](TTree& t, const Entry& en) {  //
                t.SetBranchAddress(en.name.c_str(), en.address);
            };
        } else {
            e.makeBranch = [](TTree& t, const Entry& en) -> TBranch* {  //
                return t.Branch(en.name.c_str(), static_cast<K*>(en.address));
            };
            // for stl/class branches, SetBranchAddress requires a T**
            auto pp = std::make_shared<K*>(static_cast<K*>(address));
            e.pptr = pp;
            K** raw = pp.get();
            e.setAddress = [raw](TTree& t, const Entry& en) {  //
                t.SetBranchAddress(en.name.c_str(), raw);
            };
        }
        fEntries.push_back(std::move(e));
    }

    std::vector<Entry> fEntries;
};

struct Writer {

    // will create a new `TTree` wherever `gDirectory` currently points
    Writer(Model model, std::string_view tree_name, TDirectory* dir = gDirectory)  //
        : fModel{std::move(model)} {
        if (!dir) throw std::runtime_error("Writer: null directory");
        dir->cd();
        fTree = new TTree(std::string(tree_name).c_str(), "");
        fTree->SetAutoSave(0);
        for (auto& e : fModel.fEntries) e.makeBranch(*fTree, e);
    }

    void Fill() { fTree->Fill(); }
    [[nodiscard]] TTree* GetTree() const { return fTree; }

    // if output `TFile` is owned, this must be called explicitly
    void Write() {
        if (!fTree) return;
        auto* dir = fTree->GetDirectory();
        if (!dir) return;
        dir->cd();
        fTree->Write("", TObject::kOverwrite);
    }

    Model fModel;
    TTree* fTree{nullptr};
};

struct Reader {

    Reader(Model model, std::string_view tree_name, TFile& input_file)  //
        : fModel{std::move(model)} {
        fTree = input_file.Get<TTree>(std::string(tree_name).c_str());
        if (!fTree) throw std::runtime_error("Reader: tree not found");
        for (auto& e : fModel.fEntries) e.setAddress(*fTree, e);
    }

    void Load(long long entryId) { fTree->GetEntry(entryId); }
    [[nodiscard]] long long GetEntries() const { return fTree->GetEntries(); }

    Model fModel;
    TTree* fTree{nullptr};
};

}  // namespace Framework::TeeTree
