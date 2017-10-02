clear;
%% Import Cart Data
[t_cart,dx_cart,f_cart] = ImportCartData('cart_experiment.csv',2, 1002);

%% Estimating x.. and x. for cart
T = 0.01;
N = 3; % 2*N+1 points are used as local neighbourhood for quad fit
[~,ddx_cart,~] = quadraticSavitzkyGolay(dx_cart',T,N);

%% Estimating parameters
Phi = [f_cart((N+1:end-N)) dx_cart((N+1:end-N))];
Y = ddx_cart((N+1:end-N))';

Theta = (Phi'*Phi) \ (Phi'*Y);
m = 1/Theta(1)
b = -m*Theta(2)

sim_cart = ss(-b/m, 1/m, 1, 0);
[dxsim_cart, tsim_cart, ~] = lsim(sim_cart, f_cart, t_cart);

%% Plot Results
figure;
hold on;
plot(t_cart, dx_cart, 'b-', tsim_cart, dxsim_cart, 'r--');
legend('Data', 'Sim');
grid on;
hold off;