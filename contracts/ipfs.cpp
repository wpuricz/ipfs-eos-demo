#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/multi_index.hpp>
#include <eosiolib/asset.hpp>


using namespace eosio;
using namespace std;

class ipfs : public contract {
    using contract::contract;
    
public:
    ipfs( account_name self ) :
    contract(self),
    _ipfsdata(_self,_self){}
    
    // @abi action
    void savehash(string text_hash, string image_hash) {
        // cleos push action ipfs.app savehash '["QmPnnvFkvSCmPvMdMtSNbXrFWQX5XYc4YkcVU4cujyyH8Z",""]' -p dummy
        // cleos push action ipfs.app savehash '["","QmPnnvFkvSCmPvMdMtSNbXrFWQX5XYc4YkcVU4cujyyH8Z"]' -p dummy
        // cleos get table ipfs.app ipfs.app ipfsdata
        
        auto iter = _ipfsdata.find(0);
        if(iter != _ipfsdata.end()) {
            _ipfsdata.modify(iter, 0, [&](auto& row) {
                if(text_hash != "") {
                    row.text_hash = text_hash;
                }
                if(image_hash != "") {
                    row.image_hash = image_hash;
                }
            });
        }else{
            _ipfsdata.emplace(_self, [&] (auto& row) {
                row.id = _ipfsdata.available_primary_key();
                row.text_hash = text_hash;
                row.image_hash = image_hash;
            });
        }

    }
    
    // @abi action
    void delhash(uint64_t id) {
        // cleos push action ipfs.app delhash '[0]' -p dummy
        auto iter = _ipfsdata.find(id);
        _ipfsdata.erase(iter);
    }
    

private:
    // @abi table
    struct ipfsdata {
        uint64_t id;
        string text_hash;
        string image_hash;
        
        //uint64_t primary_key() const { return id; }
        auto primary_key()const { return id; }
        
        EOSLIB_SERIALIZE(ipfsdata, (id)(text_hash)(image_hash));
    };

    multi_index<N(ipfsdata), ipfsdata> _ipfsdata;
    
    
    
};

EOSIO_ABI( ipfs, (savehash)(delhash) );
