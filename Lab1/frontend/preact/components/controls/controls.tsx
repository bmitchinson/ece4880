import { h, Component } from 'preact';
import { auth, database } from '../firebase/firebase';

import Guard from './guard/guard';
import Form from './form/form';

interface MyProps {}
interface MyState {
    currentUser: any;
    userCanWrite: boolean;
}

export default class Controls extends Component<MyProps, MyState> {
    constructor() {
        super();
        this.state = {
            currentUser: null,
            userCanWrite: false
        };
    }

    componentDidMount() {
        auth.onAuthStateChanged(currentUser => {
            this.setState({ currentUser }, this.checkIfUserCanWrite);
        });
    }

    checkIfUserCanWrite() {
        const user = this.state.currentUser;
        if (user) {
            const uid = user.uid;
            const currentUserDocRef = database.collection('auth').doc(uid);
            currentUserDocRef.get().then(doc => {
                if (doc.exists) {
                    this.setState({ userCanWrite: doc.data().canWrite });
                }
            });
        } else {
            this.setState({ userCanWrite: false });
        }
    }

    render() {
        const userSignedIn = this.state.currentUser && true;
        const userCanWrite = this.state.userCanWrite;
        return userSignedIn ? (
            <Form userCanWrite={userCanWrite} userSignedIn={userSignedIn} />
        ) : (
            <Guard />
        );
    }
}
