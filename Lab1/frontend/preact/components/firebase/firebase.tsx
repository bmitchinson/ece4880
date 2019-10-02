import app from 'firebase/app';
import 'firebase/firestore';
import 'firebase/auth';

const config = {
    apiKey: 'AIzaSyCATHhl5Tpkq1SMfoPSNqshvdScL7cj8f8',
    authDomain: 'lab1-fcb5c.firebaseapp.com',
    databaseURL: 'https://lab1-fcb5c.firebaseio.com',
    projectId: 'lab1-fcb5c',
    storageBucket: 'lab1-fcb5c.appspot.com',
    messagingSenderId: '714581628648',
    appId: '1:714581628648:web:bd495a85a5dd208a13db5d'
};

app.initializeApp(config);

export default app;

export const database = app.firestore();
export const auth = app.auth();
export const googleAuthProvider = new app.auth.GoogleAuthProvider();
