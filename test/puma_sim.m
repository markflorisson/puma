function [ P,H ] = puma_sim(P_i,H_i,t,delta_t,r,a,k,b,y,l)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 	PUMA_SIM - Matlab simulation of small cases for testing the C program							%
%                                                                                                   %
%   Author: Dante Gama                                                                              %
%	                                                                                                %
%	Input: 	P_i 		- initial puma distribution                                                 %
%			H_i 		- initial hare distribution                                                 %
%			t 			- final time at which the result is provided                                %
%			delta_t		- size of time steps                                                        %
%			r 			- intrinsic rate of hare population increase                                %
%			a 			- predation rate coefficient                                                %
%			b			- reproduction rate of predator reproduction rate per one prey eaten        %
%			m			- predator mortality rate                                                   %
%			k			- diffusion rate of hares                                                   %
%			l 			- diffusion rate of pumas                                                   %
%                                                                                                   %
%	Output:	P			- density of pumas at time t                                                %
%     		H			- density of hares at time t                                                %
%                                                                                                   %
%                                                                                                   %
%                                                                                                   %
%                                                                                                   %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


% initialize sample matrixes for testing the code

P_i = 2.1*ones(4,4);
H_i = 2.9*ones(4,4);

% Get auxiliary P_old and H_old matrixes, and initialize the solutions to zero

[m,n] = size(P_i);

H = zeros(m+2,n+2);
P = zeros(m+2,n+2);
H_old = H_i;
P_old = P_i;

%Add halos to H_old and P_old

H_old = [zeros(m,1) H_old];
H_old = [H_old zeros(m,1)];
H_old = [zeros(1,m+2);H_old];
H_old = [H_old;zeros(1,m+2)];
P_old = [zeros(m,1) P_old];
P_old = [P_old zeros(m,1)];
P_old = [zeros(1,m+2);P_old];
P_old = [P_old;zeros(1,m+2)];

%Perform the calculation

for z = 0:delta_t:t
	for i = 2:m+1
		for j = 2:n+1
			%Calculate factor N needed in the approximation equation
			if i == 2 || i == m+1 
				if j == 2 || j == n+1
					N=2;
				else
					N=3;
				end
			else
				if j == 2 || j == n+1	
					N = 3;
				else
					N=4;
				end
			end
			%Use the formulas to calculate the matrix entries	
			H(i,j) = H_old(i,j) + delta_t*(r*H_old(i,j)-a*H_old(i,j)*P_old(i,j)+k*((H_old(i-1,j)+H_old(i+1,j)+H_old(i,j-1)+H_old(i,j+1))-N*H_old(i,j)));
			P(i,j) = P_old(i,j) + delta_t*(b*H_old(i,j)*P_old(i,j)-y*P_old(i,j)+l*((P_old(i-1,j)+P_old(i+1,j)+P_old(i,j-1)+P_old(i,j+1))-N*P_old(i,j)));
		end	
	end
	H_old = H
	P_old = P
	pause
end

end

