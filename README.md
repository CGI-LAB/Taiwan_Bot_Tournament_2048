CGI-2048
========

CGI-2048 is an AI program for Game 2048. The current version is exactly the one used to participate in Taiwan 2048-Bot Tournament, http://2048-botcontest.twbbs.org/. The version was ranked the second as shown in http://2048-botcontest.twbbs.org/download/stats_2048.htm, its performance is also summarized the table below.

Our algorithm is based on the following two papers: 

[1] Szubert, M. amd Jaskowaski, W., “Temporal Difference Learning of N-tuple Networks for the Game 2048”, IEEE  CIG 2014 conference, August 2014.

[2] I-Chen Wu, Kun-Hao Yeh, Chao-Chin Liang, Chia-Chuan Chang, and Han Chiang, "Multi-Stage Temporal Difference Learning for 2048", the 2014 Conference on Technologies and Applications of Artificial Intelligence (TAAI 2014), Taipei, Taiwan, Novemeber 2014.

However, we don't have much time to refactor the code and make nice documentation. To run the program, use our trained data which can be downloaded from http://aigames.nctu.edu.tw/~cgilab/download/. The code for TD learning is not included, since the performance varies depending on the trained data anyway. 


Based on [2], we keep improving the performance. Recently, we obtained a version whose performance is better than Xi???, which was the best 2048 AI program in the world (to our knowledge), at https://github.com/nneonneo/2048-ai/pull/27. The newly improved program will be released some time later. All the performances are showned and compared in the following table.  

<table align=center>
	<tr align=center> 
		<th>Indicators</th> 
		<th>CGI-2048</th>
		<th>Our Current Program</th>
		<th>Xificurk's Program</th>
	</tr>
	
	<tr align=center> 
		<td>2048</td> 
		<td>100.0%</td>
		<td>100.0%</td> 
		<td>100.0%</td> 
	</tr> 
	<tr align=center> 
		<td>4096</td> 
		<td>100.0%</td> 
		<td>100.0%</td> 
		<td>100.0%</td> 
	</tr> 
	<tr align=center> 
		<td>8192</td> 
		<td>94%</td> 
		<td>99.5%</td> 
		<td>99.1%</td> 
	</tr> 
	<tr align=center> 
		<td>16384</td> 
		<td>59%</td>
		<td>93.6%</td> 
		<td>92.7%</td> 
	</tr> 
	<tr align=center> 
		<td>32768</td> 
		<td>0%</td> 
		<td>33.5%</td> 
		<td>31.7%</td> 
	</tr> 
	<tr align=center> 
		<td>Max score</td> 
		<td>367956</td>
		<td>833300</td> 
		<td>829300</td> 
	</tr> 
	<tr align=center> 
		<td>Ave score</td> 
		<td>251794</td>
		<td>446116</td> 
		<td>442419</td> 
	</tr> 
	<tr align=center> 
		<td>Speed</td> 
		<td>1200 moves/sec</td>
		<td>661 moves/sec</td> 
		<td>2 moves/sec</td> 
	</tr> 
</table>


