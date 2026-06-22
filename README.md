# Fusex p28

## Règles à respecter :
- Ne travaillez pas sur la branche `main`. Travaillez dans une branche à côté, puis mergez quand vous avez quelque chose qui marche.

- Pour merge : faites le directement sur gitlab, dans le menu "Merge requests". Comme ça tout le monde peut review le code avant qu'on approuve



## Pour commencer à travailler :
0 - Se faire une clé SSH si pas déjà fait. Cf plus bas

1 - Cloner le répertoire chez vous (le <repository_url> vous le trouverez sous Code -> clone with SSH ou HTTPS en fonction de votre moyen d'identification):
```
git clone <repository_url>
```

2 - Aller dans le répertoire :
```
cd <repository>
```

3 - Configurer git :
```
git config --global user.name "Your Name"
git config --global user.email "your.email@example.com"
```

4 - Créer une nouvelle branche :
```
git checkout -b name-of-branch
```

5 - Vivre votre vie, faire vos modifications

6 - Mettre vos modifications en action
```
git add .
git commit -m "Your commit message"
git push origin name-of-branch
```

7 - Si vous êtes vraiment sûr de vous, merge vos modifications sur la main branch
```
git checkout main
git pull origin main
git merge name-of-branch
git add .
git merge --continue
git commit -m "Merge branch 'name-of-branch' into main"
git push origin main
```

8 - Pour supprimer votre branche que vous avez crée si elle n'est plus utile :
```
git branch -d your-feature-branch
```


### Quoi après ?

1 - Pour retravailler sur le repo, il est très recommandé de pull toutes les nouvelles modifications
```
git pull origin main
ou
git pull <repository_url>
```
avec main étant le nom du main branch


## Créer une clé SSH

1 - Sur votre PC, ouvrez un terminal et exécutez:
```
ssh-keygen -t ed25519 -C "email@example.com"
```
en remplaçant email@example.com par votre email de CS.
Appuyer sur entrée lorsque demandé pour accepter le chemin par défaut. Pas besoin d'ajouter de mot de passe.

2 - Copiez la clé que vous venez de créer. Vous pouvez y accéder avec la commande
```
cat ~/.ssh/id_ed25519.pub
```

3 - Ajoutez-la à Gitlab:
- Allez dans Paramètres utilisateur > SSH Keys.
- Collez la clé publique dans le champ Key.
- Ajoutez un titre pour identifier la clé (par exemple, "Clé de travail").
- Cliquez sur Add Key.

4 - Quand vous clonerez le projet, il faudra copier l'adresse SSH et pas HTTPS

Si le projet est déjà sur votre ordinateur et que vous l'aviez cloné avec HTTPS, il faut remplacer l'URL distante du dépôt:
```
git remote set-url origin git@https://gitlab-research.centralesupelec.fr/louis.allix/fusex-p28.git
```