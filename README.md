CGI-2048
========

CGI-2048 is an AI program for Game 2048. The current version is exactly the one used to participate in Taiwan 2048-Bot Tournament, http://2048-botcontest.twbbs.org/. The version was ranked the second as shown in http://2048-botcontest.twbbs.org/download/stats_2048.htm, its performance is also summarized the table below: 

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

However, we don't have much time to refactor the code and make nice documentation. To run the program, use our trained data which can be downloaded from http://aigames.nctu.edu.tw/~cgilab/download/. The code for TD learning is not included, since the performance varies depending on the trained data anyway. 


Based on [2], we keep improving the performance. Recently, we obtained a version whose performance is better than Xi???, which was the best 2048 AI program in the world (to our knowledge), at https://github.com/nneonneo/2048-ai/pull/27. The newly improved program will be released some time later. All the performances are showned and compared in the following table.  

Max Score: 833300<br>
Ave Score: 446116<br>
Max Tile: 32768<br>
2048: 100% <br>
4096: 100% <br>
8192: 99.5% <br>
16384: 93.6% <br>
32768: 33.5% <br>
Speed: 661.7 moves / second. 
		<table align=center>
			<tr> 
				<th>Indicators</th> 
				<th>CGI-2048</th>
				<th>Our Current Program</th>
				<th>Xificurk's Program</th>
			</tr> 
			<tr> 
				<td>2048</td> 
				<td>100.0%</td>
				<td>100.0%</td> 
				<td>100.0%</td> 
			</tr> 
			<tr> 
				<td>4096</td> 
				<td>100.0%</td> 
				<td>100.0%</td> 
				<td>100.0%</td> 
			</tr> 
			<tr> 
				<td>8192</td> 
				<td>99.3%</td> 
				<td>100.0%</td> 
				<td>100.0%</td> 
				
			</tr> 
			<tr> 
				<td>16384</td> 
				<td>89.5%</td>
				<td>100.0%</td> 
				<td>100.0%</td> 
			</tr> 
			<tr> 
				<td>32768</td> 
				<td>27.1%</td> 
				<td>100.0%</td> 
				<td>100.0%</td> 
			</tr> 
			<tr> 
				<td>Max score</td> 
				<td>690,548</td>
				<td>100.0%</td> 
				<td>100.0%</td> 
			</tr> 
			<tr> 
				<td>Ave score</td> 
				<td>412,376</td>
				<td>100.0%</td> 
				<td>100.0%</td> 
			</tr> 
			<tr> 
				<td>Speed</td> 
				<td>About 1200 moves/sec</td>
				<td>100.0%</td> 
				<td>100.0%</td> 
			</tr> 
		</table>


