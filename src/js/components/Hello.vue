<template>
  <div class="hello">
    <h3>{{ msg }}</h3>
    <input type="file" id="files" ref="files" multiple @change="handleFilesUpload()"/>
    <button @click="submitFiles()">Submit</button>
  </div>
</template>

<script>
export default {
  name: 'hello',
  data() {
    return {
      msg: 'file upload',
      

    };
  },
  
  methods: {
    handleFilesUpload() {
      let uploadedFiles = this.$refs.files.files;
      const file = uploadedFiles[0];
      console.log(file);
      //let reader = new window.FileReader()
      //reader.readAsArrayBuffer(this.file)
      //reader.onloadend() = () => this.convertToBuffer(reader);
      /*
        Adds the uploaded file to the files array
      */
      // for( var i = 0; i < uploadedFiles.length; i++ ){
      //   this.files.push( uploadedFiles[i] );
      // }
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
