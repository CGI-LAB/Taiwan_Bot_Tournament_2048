CGI-2048
========

CGI-2048 is an AI program for Game 2048. The current version is exactly the one used to participate in Taiwan 2048-Bot Tournament, http://2048-botcontest.twbbs.org/. The version was ranked the second as shown in http://2048-botcontest.twbbs.org/download/stats_2048.htm, its performance is also summarized as follows: 

Max Score: 367956<br>
Ave Score: 251794.00<br>
Max Tile: 16384<br>
2048: 100% <br>
4096: 100% <br>
8192: 94% <br>
16384: 59% <br>

Our algorithm is based on the following two papers: 

[1] Szubert, M. amd Jaskowaski, W., “Temporal Difference Learning of N-tuple Networks for the Game 2048”, IEEE  CIG 2014 conference, August 2014.

[2] I-Chen Wu, Kun-Hao Yeh, Chao-Chin Liang, Chia-Chuan Chang, and Han Chiang, "Multi-Stage Temporal Difference Learning for 2048", the 2014 Conference on Technologies and Applications of Artificial Intelligence (TAAI 2014), Taipei, Taiwan, Novemeber 2014.

Sorry that we don't have much time to refactor the code and make it look nice in documentation. To run the program, use our trained data which can be downloaded from http://aigames.nctu.edu.tw/~cgilab/download/. The code for TD learning is not included, since the performance varies depending on the trained data anyway. 


Based on [2], we keep improving the performance. The latest performance record was: 

Max Score: 833300<br>
Ave Score: 446116<br>
Max Tile: 32768<br>
2048: 100% <br>
4096: 100% <br>
8192: 99.5% <br>
16384: 93.6% <br>
32768: 33.5% <br>
Speed: 661.7 moves / second. 

The result beats the best 2048 AI program in the world (to the best of our knowledge), at  https://github.com/nneonneo/2048-ai/pull/27. More importantly, the speed of our program is about 300 times faster. 

