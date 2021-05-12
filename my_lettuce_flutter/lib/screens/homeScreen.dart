import 'dart:async';

import 'package:firebase_database/firebase_database.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';

class HomeScreen extends StatefulWidget {
  @override
  _HomeScreenState createState() => _HomeScreenState();
}

class _HomeScreenState extends State<HomeScreen>
    with SingleTickerProviderStateMixin {
  @override
  final dbRef = FirebaseDatabase.instance.reference();
  bool value = false;
  Color color = Colors.grey;

  onUpdate() {
    setState(() {
      value = !value;
    });
  }

  static final GlobalKey<ScaffoldState> _scaffoldKey =
      new GlobalKey<ScaffoldState>();

  @override
  Widget build(BuildContext context) {
    SystemChrome.setPreferredOrientations([
      DeviceOrientation.portraitUp,
      DeviceOrientation.portraitDown,
    ]);
    return MaterialApp(
      theme: ThemeData(
        brightness: Brightness.dark,
      ),
      debugShowCheckedModeBanner: false,
      home: Scaffold(
        key: _scaffoldKey,
        drawer: Drawer(
            child: new ListView(
          children: <Widget>[
            new DrawerHeader(
              child: new Text("Ürünler"),
              decoration: new BoxDecoration(color: Colors.green),
            ),
            new ListTile(
              title: new Text("Marul"),
              onTap: () {},
            ),
            new ListTile(
              title: new Text("Çilek"),
              onTap: () {},
            ),
          ],
        )),
        body: SafeArea(
          child: StreamBuilder(
              builder: (context, snapshot) {
                if (snapshot.hasData &&
                    !snapshot.hasError &&
                    snapshot.data.snapshot.value != null) {
                  return Column(
                    children: [
                      Padding(
                        padding: const EdgeInsets.all(10.0),
                        child: Row(
                          mainAxisAlignment: MainAxisAlignment.spaceBetween,
                          children: <Widget>[
                            GestureDetector(
                              onTap: () {
                                _scaffoldKey.currentState.openDrawer();
                              },

                              child: Icon(
                                Icons.clear_all,
                                color: !value ? Colors.white : Colors.yellow,
                              ),
                              // ),
                            ),
                            Text("Marulum",
                                style: TextStyle(
                                    color: Colors.white,
                                    fontSize: 20,
                                    fontWeight: FontWeight.bold)),

                            Icon(
                              Icons.settings,
                              color: !value ? Colors.white : Colors.yellow,
                            ),
                            // ),
                          ],
                        ),
                      ),
                      SizedBox(height: 10),
                      Container(
                        color: Colors.black54,
                        child: Column(
                          mainAxisAlignment: MainAxisAlignment.center,
                          children: [
                            Row(
                              children: [
                                Padding(
                                  padding: const EdgeInsets.all(8.0),
                                  child: Text("Pompa Durumu: ",
                                      style: TextStyle(
                                          color: !value
                                              ? Colors.white
                                              : Colors.yellow,
                                          fontSize: 18,
                                          fontWeight: FontWeight.bold)),
                                ),
                                Padding(
                                  padding: const EdgeInsets.all(8.0),
                                  child: Text(
                                    snapshot.data.snapshot
                                                .value["pompa"]["statu"]
                                                .toString() ==
                                            "X"
                                        ? "Çalışıyor"
                                        : "Çalışmıyor",
                                    style: TextStyle(
                                      color: snapshot.data.snapshot
                                                  .value["pompa"]["statu"] ==
                                              "X"
                                          ? Colors.yellow
                                          : Colors.white,
                                      fontSize: 15,
                                    ),
                                  ),
                                ),
                                Padding(
                                    padding: const EdgeInsets.all(8.0),
                                    child: GestureDetector(
                                      onTap: () {
                                        pompaStatuDegis(snapshot.data.snapshot);
                                        //   readData();
                                      },
                                      child: Icon(
                                        snapshot.data.snapshot
                                                    .value["pompa"]["statu"]
                                                    .toString() ==
                                                "X"
                                            ? Icons.arrow_upward
                                            : Icons.arrow_downward,
                                        color: (snapshot.data.snapshot
                                                    .value["pompa"]["statu"]
                                                    .toString() ==
                                                "X"
                                            ? Colors.green
                                            : Colors.red),
                                      ),
                                    )),
                              ],
                            ),
                            Row(
                              children: [
                                Padding(
                                  padding: const EdgeInsets.all(8.0),
                                  child: Text("Çalışma Süresi ",
                                      style: TextStyle(
                                          color: !value
                                              ? Colors.white
                                              : Colors.yellow,
                                          fontSize: 18,
                                          fontWeight: FontWeight.bold)),
                                ),
                                Padding(
                                  padding: const EdgeInsets.all(8.0),
                                  child: Text(
                                    snapshot.data.snapshot
                                            .value["pompa"]["times"]
                                            .toString() +
                                        " saniye",
                                    style: TextStyle(
                                      fontSize: 15,
                                    ),
                                  ),
                                ),
                              ],
                            ),
                          ],
                        ),
                      ),
                      Container(
                        color: Colors.black12,
                        child: Column(
                          mainAxisAlignment: MainAxisAlignment.center,
                          children: [
                            Row(
                              children: [
                                Padding(
                                  padding: const EdgeInsets.all(8.0),
                                  child: Text("Ph Durumu: ",
                                      style: TextStyle(
                                          color: !value
                                              ? Colors.white
                                              : Colors.yellow,
                                          fontSize: 18,
                                          fontWeight: FontWeight.bold)),
                                ),
                                Padding(
                                  padding: const EdgeInsets.all(8.0),
                                  child: Text(
                                    snapshot.data.snapshot
                                                .value["phmetre"]["statu"]
                                                .toString() ==
                                            "X"
                                        ? "Çalışıyor"
                                        : "Çalışmıyor",
                                    style: TextStyle(
                                      color: snapshot.data.snapshot
                                                  .value["phmetre"]["statu"] ==
                                              "X"
                                          ? Colors.yellow
                                          : Colors.white,
                                      fontSize: 15,
                                    ),
                                  ),
                                ),
                                Padding(
                                    padding: const EdgeInsets.all(8.0),
                                    child: GestureDetector(
                                      onTap: () {
                                        phStatuDegis(snapshot.data.snapshot);
                                        //   readData();
                                      },
                                      child: Icon(
                                        snapshot.data.snapshot
                                                    .value["phmetre"]["statu"]
                                                    .toString() ==
                                                "X"
                                            ? Icons.arrow_upward
                                            : Icons.arrow_downward,
                                        color: (snapshot.data.snapshot
                                                    .value["phmetre"]["statu"]
                                                    .toString() ==
                                                "X"
                                            ? Colors.green
                                            : Colors.red),
                                      ),
                                    )),
                              ],
                            ),
                            Row(
                              children: [
                                Padding(
                                  padding: const EdgeInsets.all(8.0),
                                  child: Text("Çalışma Süresi ",
                                      style: TextStyle(
                                          color: !value
                                              ? Colors.white
                                              : Colors.yellow,
                                          fontSize: 18,
                                          fontWeight: FontWeight.bold)),
                                ),
                                Padding(
                                  padding: const EdgeInsets.all(8.0),
                                  child: Text(
                                    snapshot.data.snapshot
                                            .value["phmetre"]["times"]
                                            .toString() +
                                        " saniye",
                                    style: TextStyle(
                                      fontSize: 15,
                                    ),
                                  ),
                                ),
                              ],
                            ),
                          ],
                        ),
                      ),
                      SizedBox(height: 10),
                      Padding(
                        padding: const EdgeInsets.all(18.0),
                        child: FloatingActionButton.extended(
                          icon: value
                              ? Icon(Icons.visibility)
                              : Icon(Icons.visibility_off),
                          backgroundColor: value ? Colors.yellow : Colors.white,
                          label: value ? Text("ON") : Text("OFF"),
                          elevation: 20.00,
                          onPressed: () {
                            onUpdate();
                            writeData();
                            readData(snapshot);
                          },
                        ),
                      ),
                    ],
                  );
                } else {}
                return Container();
              },
              stream: dbRef.child("Data").onValue),
        ),
      ),
    );
  }

  Future<void> pompaStatuDegis(p_value) async {
    var p_statu = p_value.value["pompa"]["statu"].toString();
    var p_times = p_value.value["pompa"]["times"];
    if (p_statu == "X")
      await dbRef
          .child("Data")
          .child("pompa")
          .set({"statu": "", "times": p_times});
    else
      await dbRef
          .child("Data")
          .child("pompa")
          .set({"statu": "X", "times": p_times});
  }

  Future<void> phStatuDegis(p_value) async {
    var ph_high = p_value.value["high"];
    var ph_low = p_value.value["low"];
    var ph_phValue = p_value.value["phValue"];
    var ph_statu = p_value.value["statu"].toString();
    var ph_stop_times = p_value.value["stop_times"];
    var ph_times = p_value.value["times"];
    if (ph_statu == "X")
      await dbRef.child("phmetre").set({
        "high": ph_high,
        "low": ph_low,
        "phValue": ph_phValue,
        "statu": "",
        "stop_times": ph_stop_times,
        "times": ph_times
      });
    else
      await dbRef.child("phmetre").set({
        "high": ph_high,
        "low": ph_low,
        "phValue": ph_phValue,
        "statu": "X",
        "stop_times": ph_stop_times,
        "times": ph_times
      });
  }

  Future<void> writeData() async {
    await dbRef.child("LightState").set({"switch": !value});
  }

  Future<void> readData(snapshot) async {
    print("1 " + snapshot.data.snapshot.value["pompa"]["statu"].toString());
    /*
    Map<dynamic, dynamic> values = snapshot.data.snapshot.value["pompa"][];
    values.forEach((key, values) {
      print("1 " + values.toString());
    });
    print("2 " + values["statu"].toString());
    print("2 " + values.toString());
    */

    await dbRef
        .child("Data")
        .child("pompa")
        .once()
        .then((DataSnapshot snapshot) {
      //  print("1" + snapshot.toString());
      //  print("2" + snapshot.value.toString());
      //  print("3" + snapshot.value["statu"].toString());
      //print("3" + snapshot.value["pompa"].toString());
      //print("4" + snapshot.value.pompa.statu.toString());

      //print("4" + snapshot.value["pompa"].value["statu"].toString());
      // print("4" + snapshot.value["pompa[]"].toString());
      // print("5" + snapshot.value["pompa[0]"].toString());
      // print("6" + snapshot.value["pompa/statu"].toString());
    });
  }
}
