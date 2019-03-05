rm input_beams.txt output_beams0.txt
python Test_Bench_ABS2.py | python multicast_senderABS2_version_test.py
python CheckINvsOUT.py 0


