<?php
    require "MVC/Models/Pergunta.inc";

    class Gerenciador {
        // Atributos
        var $perguntas;
        var $pontuacao;
        var $nPerguntas;

        // Construtor
        public function __construct() {
            // inicia array de perguntas
            $this->perguntas = [];
            $this->pontuacao = 0;

            $arquivoJSON = file_get_contents("assets/arquivos/perguntas.json");
            $perguntasJSON = json_decode($arquivoJSON, true);

            $this->nPerguntas = count($perguntasJSON, 0);

            foreach($perguntasJSON as $p) {
                $this->perguntas[] = new Pergunta($p["enunciado"], $p["alternativas"]);
            }
        }
    
        public function carregaPergunta($id) {
            return $this->perguntas[$id];
        }

        public function buscaAlternativaCorreta($id) {
            foreach($this->perguntas[$id]->alternativas as $alternativa) {
                if($alternativa->ehCorreta) {
                    return $alternativa->letra;
                }
            }
        }

        public function autentica($login, $senha) {
            $arquivoJSON = file_get_contents("assets/arquivos/usuarios.json");
            $usuariosJSON = json_decode($arquivoJSON, true);

            foreach($usuariosJSON as $u) {
                if($u["login"] == $login && $u["senha"] == $senha) {
                    // Se o login e senha estiverem certos cria o usuario na sessão
                    $usuario = new Usuario($u["login"],$u["senha"],$u["user"],$u["email"]);
                    $_SESSION["usuario"] = $usuario;
                    return true;
                }
            }

            return false;
        }

        public function addUsuario() {
            if(isset($_POST["cadastro_login"])) {
                $caminhoArquivo = "assets/arquivos/usuarios.json";
                $usuarios = json_decode(file_get_contents($caminhoArquivo), true);
            
                // Adiciona o novo usuario no array
                $usuarios[] = array(    
                    'login' => $_POST["cadastro_login"],
                    'senha' => $_POST["cadastro_senha"],
                    'email' => $_POST["cadastro_email"],
                    'user' => $_POST["cadastro_user"]
                );
            
                // Converte novamente para o formato JSON   
                $usuariosAtualizado = json_encode($usuarios);
                file_put_contents($caminhoArquivo, $usuariosAtualizado, JSON_PRETTY_PRINT);

                unset($_POST["cadastro"]);
            }
        }
    }
?>