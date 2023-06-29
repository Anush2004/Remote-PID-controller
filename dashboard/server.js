require('dotenv').config();
const express = require('express');
const mongoose = require('mongoose');
const session = require('express-session');
const MongoStore = require('connect-mongo')(session);
const path = require('path');

const app = express();
const PORT = process.env.PORT || 3000;

// Connect to MongoDB
mongoose.connect(process.env.MONGODB_URI, {
  useNewUrlParser: true,
  useUnifiedTopology: true
})
  .then(() => {
    console.log('MongoDB Connection Succeeded.');
  })
  .catch((err) => {
    console.log('Error in DB connection: ' + err);
  });

// Set up session store
const sessionStore = new MongoStore({
  mongooseConnection: mongoose.connection
});

// Configure session middleware
app.use(session({
  secret: process.env.SESSION_SECRET || 'work hard',
  resave: true,
  saveUninitialized: true,
  store: sessionStore
}));

// Set up view engine
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'ejs');

// Parse request bodies
app.use(express.json());
app.use(express.urlencoded({ extended: false }));

// Serve static files
app.use(express.static(path.join(__dirname, 'views')));

// Define routes
const indexRouter = require('./routes/index');
app.use('/', indexRouter);

// Handle 404 errors
app.use(function (req, res, next) {
  var err = new Error('File Not Found');
  err.status = 404;
  next(err);
});

// Error handler
app.use(function (err, req, res, next) {
  res.status(err.status || 500);
  res.send(err.message);
});

// Start the server
app.listen(PORT, function () {
  console.log('Server is started on http://127.0.0.1:' + PORT);
});
