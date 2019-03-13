let app = require('express');
let express = app();


express.set('views', './views');
express.set('view engine', 'ejs');
express.use(app.static('./views'));

module.exports = express;

