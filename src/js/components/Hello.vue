<template>
  <div class="hello">
    <h3>{{ msg }}</h3>
    <!-- <input type="file" id="files" ref="files" multiple @change="handleFilesUpload()"/> -->
    <input type="file" @change="loadTextFromFile"/>
    <button @click="submitFiles()">Submit</button>
    
    <div>
      <!-- <input type="text" name="ipfstext" v-model="ipfstext"/>
      <button @click="submitText()">Submit</button> -->
      <!-- <button @click="display()">Display Text</button> -->
      <!-- <div>Saved Text: {{ipfsSavedText}}</div> -->
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
const IPFS = require('ipfs-api');
const ipfs = new IPFS({ host: "ipfs.infura.io", port: 5001, protocol: "https" });
//https://alligator.io/vuejs/file-reader-component/
//https://forums.asp.net/t/1977558.aspx?HTML5+fileReader+onloadend+event+not+getting+fire+with+while+loop+loop

export default {
  name: 'hello',
  data() {
    return {
      msg: 'file upload',
      ipfstext:"",
      ipfsSavedText:""
    };
  },
  
  methods: {
    loadTextFromFile(ev) {
      const file = ev.target.files[0];
      const reader = new FileReader();

      //reader.onload = e => this.$emit("load", e.target.result);
      //reader.onload = e => console.log(e.target.result);
      //reader.onload = e => console.log(e.target.result);
      //reader.readAsText(file);
      let hash = "QmQ9UsE5S68bDZn4ne8xu55qMTjioL5bfvQmdHcT65Hvsv"
      reader.onload = async () => {
        var result = await ipfs.add([Buffer.from(reader.result)]);
        console.log(result[0].hash);
        console.log("finished on load");
      };
      reader.readAsArrayBuffer(file);

    },
    handleFilesUpload() {
      let uploadedFiles = this.$refs.files.files;
      const file = uploadedFiles[0];
      console.log(file);
      let reader = new window.FileReader()
      //reader.onloadend = () => this.saveToIpfs(reader);
      reader.onload = function(e) {
        console.log("on load ccalled");
      }
      reader.readAsArrayBuffer(this.file);
      console.log("finishd handle method");
      //reader.onloadend() = () => this.convertToBuffer(reader);
      /*
        Adds the uploaded file to the files array
      */
      // for( var i = 0; i < uploadedFiles.length; i++ ){
      //   this.files.push( uploadedFiles[i] );
      // }
    },
    saveFileToIpfs(reader) {

      console.log("entered save to ipfs");
      let ipfsId
      const buffer = Buffer.from(reader.result)
      console.log("calling ipfs");
      ipfs.add(buffer, { progress: (prog) => console.log(`received: ${prog}`) })
      .then((response) => {
        console.log(response)
        ipfsId = response[0].hash
        console.log(ipfsId)
        //this.setState({added_file_hash: ipfsId})
        alert("added file hash: " + ipfsId);
      }).catch((err) => {
        console.error(err)
      })

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
      }catch(e) {
        console.log("error saving to ipfs and blockchain: " + e);
      }

    },
    async display() {
      // buffer: true results in the returned result being a buffer rather than a stream
      let rows = await this.getHashFromBlockchain();
      let hash = rows[0].text_hash;
      //let hash = "QmPnnvFkvSCmPvMdMtSNbXrFWQX5XYc4YkcVU4cujyyH8Z";
      ipfs.cat(hash, (err, data) => {
        if (err) { return console.error('ipfs cat error', err) }
        this.ipfsSavedText = data.toString('utf8');
        
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
        //this.processing = false;

      }catch(e) {
        alert(JSON.parse(e).error.what)
        
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
</style>
