<template>
  <div class="hello">
    
    <div class="text-section">
    
      <h4>Save Text to IPFS</h4>
      <input type="text" name="ipfstext" v-model="ipfstext"/>
      <button @click="submitText()">Submit</button>
      <div>Hash:{{textSavedHash}}</div>
      <div>Saved Text: {{ipfsSavedText}}</div>
    
    </div>

    <div class="image-section">
    
      <h4>Save Image to IPFS</h4>
      <input type="file" @change="uploadImage"/>
      <br/>
      <div>Hash:{{imageSavedHash}}</div>
      <img id="ipfsImage"   />
    
    </div>

  </div>
</template>

<script>
// EOS Initialization and Config
import * as Eos from 'eosjs';
import secrets from '../../../config/secrets'

let config = {
  keyProvider:secrets.keys,
  httpEndpoint: secrets.endpoint
};
let eos = Eos(config); // 127.0.0.1:8888
let appcode = 'ipfs.app';

// IPFS Initialization
const ipfsHost = "ipfs.infura.io";
const ipfsHttp = "https";
const ipfsPort = 5001;

const IPFS = require('ipfs-api');
const ipfs = new IPFS({ host: ipfsHost, port: ipfsPort, protocol: ipfsHttp });

/*
Credit to 
https://github.com/ambrosus/ipfs-image-web-upload
for handling the image upload
*/
export default {
  name: 'ips-eos-demo',
  data() {
    return {
      msg: 'file upload',
      ipfstext:"",
      ipfsSavedText:"",
      imageSavedHash:"",
      textSavedHash:""
    };
  },
  async created() {
    const rows = await this.getHashFromBlockchain();
    this.displayImage(rows[0].image_hash);
    this.displayText(rows[0].text_hash);
  },
  methods: {
    uploadImage(ev) {
      const file = ev.target.files[0];
      const reader = new FileReader();
      
      reader.onload = async () => {
        var result = await ipfs.add([Buffer.from(reader.result)]);
        let imageHash = result[0].hash;
        console.log("finished on load:" + imageHash);
        await this.saveHashToBlockchain("",imageHash);
        this.displayImage(imageHash);  
      };
      reader.readAsArrayBuffer(file);
    },

    displayImage(hash) {
      document.getElementById('ipfsImage').src = ipfsHttp + "://" + ipfsHost + "/ipfs/" + hash;
      this.imageSavedHash = hash;
    },

    async submitText() {

      if(this.ipfstext === "") {
        console.log("text is blank");
        return;
      }
      var buf = Buffer.from(this.ipfstext, 'utf8');
      
      try {
          let response = await ipfs.add(buf)
          let hash = response[0].hash;
          console.log("Hash from IPFS: " + hash);
          await this.saveHashToBlockchain(hash,"");
          let rows = await this.getHashFromBlockchain();
          this.displayText(rows[0].text_hash);
      }catch(e) {
        console.log("error saving to ipfs and blockchain: " + e);
      }

    },
    async displayText(hash) {
      // fetch text from ipfs
      ipfs.cat(hash, (err, data) => {
        if (err) { return console.error('ipfs cat error', err) }
        this.ipfsSavedText = data.toString('utf8');
        this.textSavedHash = hash;
      })
    },

    async getHashFromBlockchain() {
      let tableQuery = {
        "json":true,
        "scope":appcode,
        "code":appcode,
        "table":"ipfsdata"
      }
      
      let result = await eos.getTableRows(tableQuery);
      console.log(result.rows);
      return result.rows;
    },

    async saveHashToBlockchain(textHash,imageHash) {
      try {
        this.processing = true;
        let response = await eos.transaction({
          actions: [
            {
              account: appcode,
              name: 'savehash',
              authorization: [{
                actor: "dummy",
                permission: 'active'
              }],
              data: {
                text_hash:textHash,
                image_hash:imageHash
              }
            }
          ]
        })
        console.log(response)
        return response;

      }catch(e) {
        console.log(JSON.parse(e).error.what)
        
      }
    }

  }
};
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
h1, h2 {
  font-weight: normal;
}

ul {
  list-style-type: none;
  padding: 0;
}

li {
  display: inline-block;
  margin: 0 10px;
}

a {
  color: #42b983;
}
img {
  width: 300px;
  height: 300px;
}
.image-section {
  width:500px;
  height: 500px;
  border: 1px;
  border-style: solid;
  
}
.text-section {
  width:500px;
  height: 150px;
  border: 1px;
  border-style: solid;
  margin-bottom: 20px;
}

</style>
