import { h, Component } from 'preact';

import { auth, googleAuthProvider } from '../firebase/firebase';

interface MyProps {}
interface MyState {
    currentUser: any;
}

export default class Controls extends Component<MyProps, MyState> {
    constructor() {
        super();
        this.state = {
            currentUser: null
        };
    }

    componentDidMount() {
        auth.onAuthStateChanged(currentUser => {
            if (currentUser) {
                this.setState({ currentUser }, () => console.log('Logged In!'));
            } else {
                this.setState({ currentUser }, () => console.log('Logged Out!'));
            }
        });
    }

    // <button onClick={() => auth.signInWithRedirect(googleAuthProvider)}>Sign In</button>
    // {user && <p>Signed In As: {name}</p>}
    // <button onClick={() => auth.signOut()}>Sign Out</button>

    render() {
        // const user = this.state.currentUser;
        // const name = user ? user.displayName : 'No one';
        return <h2>Controls</h2>;
    }
}
