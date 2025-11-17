class TestModel {
  String camera;
  int flame; // 0 or 1 instead of bool
  int motion; // 0 or 1 instead of bool
  int studentCount;

  TestModel({
    required this.camera,
    required this.flame,
    required this.motion,
    required this.studentCount,
  });

  // Convert Firebase snapshot to a TestModel object
  factory TestModel.fromMap(Map<dynamic, dynamic> map) {
    return TestModel(
      camera: map['camera'] ?? '',
      flame: (map['flame'] ?? 0) is bool
          ? ((map['flame'] == true) ? 1 : 0)
          : map['flame'],
      motion: (map['motion'] ?? 0) is bool
          ? ((map['motion'] == true) ? 1 : 0)
          : map['motion'],
      studentCount: map['Student_count'] ?? 0,
    );
  }

  // Convert the model back to a map (for Firebase)
  Map<String, dynamic> toMap() {
    return {
      'camera': camera,
      'flame': flame,
      'motion': motion,
      'Student_count': studentCount,
    };
  }
}
