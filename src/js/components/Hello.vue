<template>
  <div class="hello">
    <h3>{{ msg }}</h3>
    <!-- <input type="file" id="files" ref="files" multiple @change="handleFilesUpload()"/> -->
    <!-- <input type="file" @change="loadTextFromFile"/> -->
    <input type="text" name="ipfstext" v-model="ipfstext"/>
    <button @click="submitFiles()">Submit</button>
    <button @click="display()">Display Text</button>
    <div>Saved Text: {{ipfsSavedText}}</div>
  </div>
</template>

<script>
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

      reader.onload = e => this.$emit("load", e.target.result);
      reader.readAsText(file);
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
    saveToIpfs(reader) {
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
    submitFiles() {
      if(this.ipfstext === "") {
        console.log("text is blank");
        return;
      }
      var buf = Buffer.from(this.ipfstext, 'utf8');
      ipfs.add(buf, (err, res) => {
        if(err) {
          console.log("error saving to ipfs: " + err);
        }

        res.forEach((item) => {
          if (item && item.hash) {
            console.log('successfully stored', item.hash)
            //display(item.hash)
          }
        })
      })
    },
    display() {
      // buffer: true results in the returned result being a buffer rather than a stream
      let hash = "QmPnnvFkvSCmPvMdMtSNbXrFWQX5XYc4YkcVU4cujyyH8Z";
      ipfs.cat(hash, (err, data) => {
        if (err) { return console.error('ipfs cat error', err) }
        this.ipfsSavedText = data.toString('utf8');
        
      })
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
