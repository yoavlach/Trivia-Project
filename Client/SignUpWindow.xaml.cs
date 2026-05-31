using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace Client
{
    /// <summary>
    /// Interaction logic for SignUpWindow.xaml
    /// </summary>
    public partial class SignUpWindow : Window
    {
        public SignUpWindow()
        {
            InitializeComponent();
        }

        private void backToLoginButtonClicked(object sender, RoutedEventArgs e)
        {
            LoginWindow loginWindow = new LoginWindow();
            loginWindow.Show();
            Close();
        }

        private void signupButtonClicked(object sender, RoutedEventArgs e)
        {
            byte[] serializedSignupRequest = JsonRequestPacketSerializer.serializeRequest(new SignupRequest
            {
                username = usernameInput.Text,
                password = passwordInput.Password,
                email = emailInput.Text
            });
            byte[] serverResp = Communicator.sendAndReceive(serializedSignupRequest);
            try
            {
                switch (Helper.extractMessageCode(serverResp))
                {
                    case (int)MESSAGE_CODES.SIGN_UP:
                        SignupResponse signupResponse = JsonResponsePacketDeserializer.deserializeSignupResponse(serverResp);
                        if ((int)signupResponse.status == (int)STATUSES.SUCCESS)
                        {
                            MainMenu mainMenu = new MainMenu();
                            mainMenu.Show();
                            Close();
                        }
                        else
                            MessageBox.Show(StatusMessages.Map[(int)signupResponse.status]);
                        break;
                    case (int)STATUSES.FAILURE:
                        ErrorResponse errorResponse = JsonResponsePacketDeserializer.deserializeErrorResponse(serverResp);
                        MessageBox.Show(errorResponse.message);
                        break;
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Unexpected Error: " + ex.Message);
                return;
            }
        }

        private void usernameInput_TextChanged(object sender, TextChangedEventArgs e)
        {

        }
    }
}