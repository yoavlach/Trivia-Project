using System.Net;
using System.Net.Sockets;
using System.Runtime.CompilerServices;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Client
{
    /// <summary>
    /// Interaction logic for LoginWindow.xaml
    /// </summary>
    public partial class LoginWindow : Window
    {
        private string SERVER_IP = "127.0.0.1";
        private int SERVER_PORT = 6767;

        /**
         * Initializes the login window component and sets up the socket connection to the server
         * Note: Shows an error message and closes the window if the connection fails
         */
        public LoginWindow()
        {
            InitializeComponent();
            if (!Communicator.m_socketIntialized)
            {
                TcpClient m_client = new TcpClient();
                try
                {
                    IPEndPoint serverEndPoint = new IPEndPoint(IPAddress.Parse(SERVER_IP), SERVER_PORT);
                    m_client.Connect(serverEndPoint);
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                    Close();
                }
                Communicator.m_stream = m_client.GetStream();
                Communicator.m_client = m_client;
            }
        }

        /**
         * Handles the action when the sign up button is clicked
         * Note: Opens the sign up window and closes the current login window
         * @param sender the object that triggered the event
         * @param e the event arguments
         */
        private void signUpButtonClicked(object sender, RoutedEventArgs e)
        {
            SignUpWindow signupWindow = new SignUpWindow();
            signupWindow.Show();
            Close();
        }

        /**
         * Handles the action when the close button is clicked
         * Note: Exits the entire application
         * @param sender the object that triggered the event
         * @param e the event arguments
         */
        private void closeButtonClicked(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }

        /**
         * Handles the action when the login button is clicked by sending a login request to the server
         * Note: Opens the main menu on a successful login or displays an error message if the login fails
         * @param sender the object that triggered the event
         * @param e the event arguments
         */
        private void loginButtonClicked(object sender, RoutedEventArgs e)
        {
            byte[] serializedLoginRequest = JsonRequestPacketSerializer.serializeRequest(new LoginRequest
            {
                username = usernameInput.Text,
                password = passwordInput.Password
            });
            byte[] serverResp = Communicator.sendAndReceive(serializedLoginRequest);
            try
            {
                switch (Helper.extractMessageCode(serverResp))
                {
                    case (int)MESSAGE_CODES.LOG_IN:
                        LoginResponse loginResponse = JsonResponsePacketDeserializer.deserializeLoginResponse(serverResp);
                        if ((int)loginResponse.status == (int)STATUSES.SUCCESS)
                        {
                            MainMenu mainMenu = new MainMenu();
                            mainMenu.Show();
                            this.Close();
                        }
                        else
                            MessageBox.Show(StatusMessages.Map[(int)loginResponse.status]);
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
    }
}