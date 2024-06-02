#include <QDebug>
#include <app-backend/app-backend.hpp>
#include <thread>
#include <util/guard.hpp>

AppBackend::AppBackend(int argc, char* argv[]) {
  config_.read(argc, argv);
  auth_service_ = std::make_unique<user_service::AuthService::Stub>(
      grpc::CreateChannel(config_.rpc_map.user_service.uri + ":" +
                              std::to_string(config_.rpc_map.user_service.port),
                          grpc::InsecureChannelCredentials()));
}

void AppBackend::signIn(const QString& username, const QString& password) {
  user_service::SignInRequest request{};

  if (auth_service_ == nullptr)
  {
    qDebug() << "Error: Auth service not inited";
    emit showError("Error: Auth service not inited");
    return;
  }

  request.set_login(username.toStdString());
  request.set_password(password.toStdString());

  if (worker_.joinable()) worker_.join();
  worker_ = std::thread([this](const user_service::SignInRequest request) {
      Guard g(std::bind(&AppBackend::closeLoadingPopup, this));
      emit openLoadingPopup();

      grpc::ClientContext context{};
      user_service::SignInResponse response{};

      auto status = auth_service_->SignIn(&context, request, &response);

      if (!status.ok()) {
        qDebug() << "Error " << status.error_code() << ":"
                 << status.error_message();
        emit showError(QString::fromStdString("Error " +
                                        std::to_string(status.error_code()) +
                                        ":" + status.error_message()));
      } else {
        qDebug() << "Success. Token - " << response.token();
        emit signInDone();
      }
  }, std::move(request));
}

void AppBackend::signUp(const QString& username, const QString& password) {
  user_service::SignUpRequest request{};

  if (auth_service_ == nullptr) {
    qDebug() << "Error: Auth service not inited";
    emit showError("Error: Auth service not inited");
    return;
  }

  request.set_login(username.toStdString());
  request.set_password(password.toStdString());

  if (worker_.joinable()) worker_.join();
  worker_ = std::thread([this](const user_service::SignUpRequest request) {
    Guard g(std::bind(&AppBackend::closeLoadingPopup, this));
    emit openLoadingPopup();

    grpc::ClientContext context{};
    user_service::SignUpResponse response{};

    auto status = auth_service_->SignUp(&context, request, &response);
    if (!status.ok()) {
      qDebug() << "Error " << status.error_code() << ":"
               << status.error_message();
      emit showError(QString::fromStdString("Error " +
                                       std::to_string(status.error_code()) +
                                       ":" + status.error_message()));
    } else {
      qDebug() << "Success. Token - " << response.token();
      emit signUpDone();
    }
  }, std::move(request));
}
