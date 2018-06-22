module.exports = {
  mode: "development",
  entry: "./lib/js/src/index.js",
  output: {
    filename: "dist/bundle.js"
  },
  devServer: {
    publicPath: "/"
  }
};
