import 'package:flutter/material.dart';
import 'package:firebase_core/firebase_core.dart';
import 'package:firebase_database/firebase_database.dart';
import 'package:secu/firebase_options.dart';
import 'package:url_launcher/url_launcher.dart';
import 'test_model.dart'; // Import the model

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await Firebase.initializeApp(
    options: DefaultFirebaseOptions.currentPlatform,
  );
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      theme: ThemeData.dark().copyWith(
        scaffoldBackgroundColor: Colors.black,
      ),
      home: FirebaseDataScreen(),
    );
  }
}

class FirebaseDataScreen extends StatefulWidget {
  @override
  _FirebaseDataScreenState createState() => _FirebaseDataScreenState();
}

class _FirebaseDataScreenState extends State<FirebaseDataScreen> {
  final DatabaseReference _database = FirebaseDatabase.instance.ref("test");
  TestModel? _testData;

  @override
  void initState() {
    super.initState();

    // Listen for real-time updates
    _database.onValue.listen((DatabaseEvent event) {
      if (event.snapshot.value != null) {
        setState(() {
          _testData = TestModel.fromMap(event.snapshot.value as Map);
        });
      }
    });
  }

  // Function to open URL
  Future<void> _openLink(String url) async {
    Uri uri = Uri.parse(Uri.encodeFull(url)); // Encode special characters
    if (await canLaunchUrl(uri)) {
      await launchUrl(uri, mode: LaunchMode.platformDefault);
    } else {
      ScaffoldMessenger.of(context).showSnackBar(
        SnackBar(content: Text("Could not open the link")),
      );
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text("Firebase Test Data"),
        backgroundColor: Colors.blueGrey,
      ),
      body: Center(
        child: _testData == null
            ? CircularProgressIndicator()
            : Column(
                mainAxisAlignment: MainAxisAlignment.center,
                children: [
                  GestureDetector(
                    onTap: () => _openLink(_testData!.camera),
                    child: Container(
                      padding: EdgeInsets.all(16),
                      margin:
                          EdgeInsets.symmetric(vertical: 10, horizontal: 20),
                      decoration: BoxDecoration(
                        color: Colors.blue,
                        borderRadius: BorderRadius.circular(12),
                        boxShadow: [
                          BoxShadow(
                            color: Colors.blueAccent.withOpacity(0.5),
                            blurRadius: 8,
                            spreadRadius: 2,
                          ),
                        ],
                      ),
                      child: Text(
                        "Camera: ${_testData!.camera}",
                        style: TextStyle(
                          fontSize: 20,
                          color: Colors.white,
                          fontWeight: FontWeight.bold,
                        ),
                      ),
                    ),
                  ),
                  _buildInfoCard(
                    text:
                        _testData!.flame == 0 ? "Fire in Building" : "No Fire",
                    color: _testData!.flame == 0 ? Colors.red : Colors.green,
                  ),
                  _buildInfoCard(
                    text: _testData!.motion == 1
                        ? "Motion Detected"
                        : "No Motion",
                    color: _testData!.motion == 1 ? Colors.red : Colors.green,
                  ),
                  _buildStudentCountCard(),
                ],
              ),
      ),
    );
  }

  Widget _buildInfoCard({required String text, required Color color}) {
    return Container(
      padding: EdgeInsets.all(16),
      margin: EdgeInsets.symmetric(vertical: 10, horizontal: 20),
      decoration: BoxDecoration(
        color: color,
        borderRadius: BorderRadius.circular(12),
        boxShadow: [
          BoxShadow(
            color: color.withOpacity(0.5),
            blurRadius: 8,
            spreadRadius: 2,
          ),
        ],
      ),
      child: Text(
        text,
        style: TextStyle(
          fontSize: 20,
          color: Colors.white,
          fontWeight: FontWeight.bold,
        ),
        textAlign: TextAlign.center,
      ),
    );
  }

  Widget _buildStudentCountCard() {
    return Container(
      padding: EdgeInsets.all(16),
      margin: EdgeInsets.symmetric(vertical: 10, horizontal: 20),
      decoration: BoxDecoration(
        color: Colors.blue,
        borderRadius: BorderRadius.circular(12),
        boxShadow: [
          BoxShadow(
            color: Colors.blueAccent.withOpacity(0.5),
            blurRadius: 8,
            spreadRadius: 2,
          ),
        ],
      ),
      child: Column(
        children: [
          Text(
            "Student Count",
            style: TextStyle(
              fontSize: 16,
              color: Colors.white,
              fontWeight: FontWeight.bold,
            ),
          ),
          SizedBox(height: 5),
          Text(
            "${_testData!.studentCount}",
            style: TextStyle(
              fontSize: 24,
              color: Colors.white,
              fontWeight: FontWeight.bold,
            ),
          ),
        ],
      ),
    );
  }
}
