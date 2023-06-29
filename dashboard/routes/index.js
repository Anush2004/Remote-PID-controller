var express = require("express");
var router = express.Router();
var User = require("../models/user");

router.get("/", function (req, res, next) {
  return res.render("index.ejs");
});

router.post("/", function (req, res, next) {
  // console.log(req.body);
  var personInfo = req.body;

  if (!personInfo.email || !personInfo.password) {
    res.send();
  } else {
    User.findOne({ email: personInfo.email }, function (err, data) {
      if (!data) {
        var c;
        User.findOne({}, function (err, data) {
          if (data) {
            // console.log("if");
            c = data.unique_id + 1;
          } else {
            c = 1;
          }

          var newPerson = new User({
            unique_id: c,
            email: personInfo.email,
            password: personInfo.password,
          });

          newPerson.save(function (err, Person) {
            if (err) console.log(err);
            else console.log("Success");
          });
        })
          .sort({ _id: -1 })
          .limit(1);
        res.send({ Success: "You are registered, you can login now." });
        console.log("Success sent");
      } else {
        res.send({ Success: "Email already exists." });
        console.log("Success sent");
      }
    });
  }
});


router.get("/login", function (req, res, next) {
  return res.render("login.ejs");
});



router.post("/login", function (req, res, next) {
  console.log(req.body);
  User.findOne({ email: req.body.email }, function (err, data) {
    if (data) {
      if (data.password == req.body.password) {
        console.log("Done Login");
        req.session.userId = data.unique_id;
        console.log(req.session.userId);
        res.send({ Success: "Success!" });
        console.log("Success sent");
      } else {
        console.log("Login not done");
        res.send({ Success: "Wrong password!" });
      }
    } else {
      console.log("Login not done2");
      res.send({ Success: "This Email Is not registered!" });
    }
  });
});

router.get("/profile", function (req, res, next) {
  console.log("profile");
  User.findOne({ unique_id: req.session.userId }, function (err, data) {
    console.log("data");
    //console.log(unique_id);
    console.log(req.session.userId);
    console.log(data);
    if (!data) {
      res.redirect("/");
    } else {
      console.log("found");
      return res.render("data.ejs");
    }
  });
});

router.get("/logout", function (req, res, next) {
  console.log("logout");
  if (req.session) {
    // delete session object
    req.session.destroy(function (err) {
      if (err) {
        return next(err);
      } else {
        return res.redirect("/");
      }
    });
  }
});

router.get("/forgetpass", function (req, res, next) {
  res.render("forget.ejs");
});

router.get("/theory", function (req, res, next) {
  res.render("theory.ejs");
});

router.get("/experiment", function (req, res, next) {
  res.render("experiments.ejs");
});

router.get("/error", function (req, res, next) {
  res.render("pagenotexist.ejs");
});

router.post("/forgetpass", function (req, res, next) {
  //console.log('req.body');
  //console.log(req.body);
  User.findOne({ email: req.body.email }, function (err, data) {
    console.log(data);
    if (!data) {
      res.send({ Success: "This Email Is not registered!" });
    } else {
      // res.send({"Success":"Success!"});
      data.password = req.body.password;
      //data.passwordConf=req.body.passwordConf;

      data.save(function (err, Person) {
        if (err) console.log(err);
        else console.log("Success");
        res.send({ Success: "Password changed!" });
      });
    }
  });
});

module.exports = router;
